#include "ResourceManager.h"
#include "GameInstance.h"
#include "ComputeShader.h"
#include "FilesystemManager.h"
#include "GraphicsGL.h"
#include <regex>
#include <boost/algorithm/string/erase.hpp>

//If the following define exists, the engine will create some default resource files if the folder structure is missing.
//I opt to create and then read files rather than reading directly from the resources because the generated files have comments in them that explain how they work
//They are also easier to just modify instead of writing new ones from scratch
//#define VE_CREATE_DEFAULT_RESOURCES

void ResourceManager::GenerateDefaultTextures()
{
	std::vector<unsigned char>& pixels = std::vector<unsigned char>
	{
		255, 0, 255, 255,	 0, 0, 0, 255,
		0, 0, 0, 255,	 255, 0, 255, 255
	};

	_cachedTexture.Add("base_texture", _graphics->CreateTexture("base_texture", pixels, glm::ivec2(2, 2)), true);

	pixels = std::vector<unsigned char>{ 0, 0, 0, 255 };
	_cachedTexture.Add("black", _graphics->CreateTexture("black", pixels, glm::ivec2(1, 1)), true);

	pixels = std::vector<unsigned char>{ 255, 255, 255, 255 };
	_cachedTexture.Add("white", _graphics->CreateTexture("white", pixels, glm::ivec2(1, 1)), true);
}

void ResourceManager::LoadDefaultResources()
{
	//Base shaders
	GetSurfaceShader("Shaders/Base/2D");
	GetSurfaceShader("Shaders/Base/Screen");

	GetComputeShader("Shaders/Base/Test_FB");

	//Base meshes
	GetMesh("Meshes/Base/quad.vm");
	GetMesh("Meshes/Base/screenQuad.vm");

	//Base materials
	GetMaterial("Materials/Base/Screen.vmat");

	//Fonts
	GetFont("Fonts/Amble.ttf");
}

void ResourceManager::PreprocessTextSource(std::string& inoutShaderSource)
{
	inoutShaderSource = std::regex_replace(inoutShaderSource, std::regex(R"(\/\/.*$)"), "");

	std::regex noparseRegex(R"(^\s*\#pragma noparse\s*$)");
	if(std::regex_match(inoutShaderSource, noparseRegex))
	{
		inoutShaderSource = std::regex_replace(inoutShaderSource, noparseRegex, "");
		return;
	}

	const std::string& includeSignature = "#pragma include";
	size_t index = inoutShaderSource.find(includeSignature);
	while(index != std::string::npos)
	{
		size_t nextNewline = inoutShaderSource.find('\n', index);
		if(nextNewline == std::string::npos)
		{
			nextNewline = inoutShaderSource.size();
		}

		const std::string includeDir = inoutShaderSource.substr(index + includeSignature.size() + 1, nextNewline - index - includeSignature.size() - 1);
		const std::string includeContents = *GetTextData(includeDir);

		inoutShaderSource
			= inoutShaderSource.substr(0, index)
			.append("\n")
			.append(includeContents)
			.append(inoutShaderSource.substr(nextNewline, inoutShaderSource.size() - nextNewline));

		index = inoutShaderSource.find(includeSignature);
	}
}

template<>
std::unique_ptr<Texture> ResourceManager::CreateResource(const std::string& key)
{
	glm::ivec2 textureSize;
	std::vector<unsigned char> textureData;
	_filesystem->LoadTextureData(key, textureData, textureSize);
	return _graphics->CreateTexture(key, textureData, textureSize);
}

template<>
std::unique_ptr<SurfaceShader> ResourceManager::CreateResource(const std::string& key)
{
	std::string vertSource = _filesystem->ReturnFile(key + ".vert");
	PreprocessTextSource(vertSource);
	std::string fragSource = _filesystem->ReturnFile(key + ".frag");
	PreprocessTextSource(fragSource);

	return	_graphics->CreateShader<SurfaceShader>(
		key,
		std::vector<ShaderAttachment>{
		ShaderAttachment(vertSource, GL_VERTEX_SHADER),
			ShaderAttachment(fragSource, GL_FRAGMENT_SHADER)});
}

template<>
std::unique_ptr<ComputeShader> ResourceManager::CreateResource(const std::string& key)
{
	std::string shaderSource = _filesystem->ReturnFile(key + ".comp");
	PreprocessTextSource(shaderSource);

	return _graphics->CreateShader<ComputeShader>(key, std::vector<ShaderAttachment>{
		ShaderAttachment(shaderSource, GL_COMPUTE_SHADER)
	});
}

template<>
inline std::unique_ptr<Mesh> ResourceManager::CreateResource(const std::string& key)
{
	CachedMesh* cachedMesh = GetCachedMesh(key);
	return _graphics->CreateMesh(key, cachedMesh);
}

template<>
inline std::unique_ptr<Font> ResourceManager::CreateResource(const std::string& key)
{
	return _graphics->CreateFont(key);
}

template<>
inline std::unique_ptr<std::string> ResourceManager::CreateResource(const std::string& key)
{
	std::unique_ptr<std::string> resource = _filesystem->LoadFileResource<std::string>(key);
	PreprocessTextSource(*resource);
	return resource;
}

void ResourceManager::Init()
{
	_debug = _serviceManager->Debug();
	_graphics = _serviceManager->Graphics();
	_filesystem = _serviceManager->Filesystem();

#ifdef VE_CREATE_DEFAULT_RESOURCES
	if(!FS::exists("Meshes/") || !FS::exists("Shaders/") || !FS::exists("Settings/") || !FS::exists("States/"))
	{
		_debug->VE_LOG("-----\n\n\nFile structure invalid. Creating default resources.\n\n\n-----");
		ResourceInitializer::Init();
	}
#endif

	GenerateDefaultTextures();
	LoadDefaultResources();
}

void ResourceManager::Update() {}

void ResourceManager::Cleanup()
{
	Unload(true);
}

void ResourceManager::Unload(bool includePersistent)
{
	_cachedComputeShader.Cleanup(includePersistent);
	_cachedFont.Cleanup(includePersistent);
	_cachedJsonData.Cleanup(includePersistent);
	_cachedMaterial.Cleanup(includePersistent);
	_cachedMesh.Cleanup(includePersistent);
	_cachedCachedMesh.Cleanup(includePersistent);
	_cachedPostEffect.Cleanup(includePersistent);
	_cachedSurfaceShader.Cleanup(includePersistent);
	_cachedTextData.Cleanup(includePersistent);
	_cachedTexture.Cleanup(includePersistent);
}

ResourceManager::ResourceManager(GameInstance* serviceManager) : BaseService(serviceManager)
{

}

ResourceManager::~ResourceManager()
= default;
