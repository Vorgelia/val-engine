#include "ResourceManager.h"
#include "ServiceManager.h"
#include "Mesh.h"
#include "SurfaceShader.h"
#include "ComputeShader.h"
#include "FilesystemManager.h"
#include "Material.h"
#include "Texture.h"
#include "PostEffect.h"
#include "Font.h"
#include "ResourceInitializer.h"
#include "DebugLog.h"
#include "CachedMesh.h"
#include "GraphicsGL.h"

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

	_cachedTexture.Add("base_texture", _graphics->CreateTexture("base_texture", pixels, glm::ivec2(2, 2), GL_RGBA16, GL_NEAREST, GL_REPEAT), true);

	pixels = std::vector<unsigned char>{ 0, 0, 0, 255 };
	_cachedTexture.Add("black", _graphics->CreateTexture("black", pixels, glm::ivec2(1, 1), GL_RGBA16, GL_NEAREST, GL_REPEAT), true);

	pixels = std::vector<unsigned char>{ 255, 255, 255, 255 };
	_cachedTexture.Add("white", _graphics->CreateTexture("white", pixels, glm::ivec2(1, 1), GL_RGBA16, GL_NEAREST, GL_REPEAT), true);
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

void ResourceManager::PreprocessShaderSource(std::string& inoutShaderSource)
{
	const std::string& includeSignature = "#pragma include";
	size_t index = inoutShaderSource.find(includeSignature);
	while(index != std::string::npos)
	{
		size_t nextNewline = inoutShaderSource.find('\n', index);
		if(nextNewline == std::string::npos)
		{
			nextNewline = inoutShaderSource.size();
		}

		std::string includeDir = inoutShaderSource.substr(index + includeSignature.size() + 1, nextNewline - index - includeSignature.size() - 1);
		std::string includeContents = *GetTextData(includeDir);

		inoutShaderSource
			= inoutShaderSource.substr(0, index)
			+ "\n"
			+ includeContents
			+ inoutShaderSource.substr(nextNewline, inoutShaderSource.size() - nextNewline);

		index = inoutShaderSource.find(includeSignature);
	}
}


template<>
std::unique_ptr<SurfaceShader> ResourceManager::CreateResource(const std::string& key)
{
	std::string vertSource = _filesystem->ReturnFile(key + ".vert");
	PreprocessShaderSource(vertSource);
	std::string fragSource = _filesystem->ReturnFile(key + ".frag");
	PreprocessShaderSource(fragSource);

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
	PreprocessShaderSource(shaderSource);

	return _graphics->CreateShader<ComputeShader>(key, std::vector<ShaderAttachment>{
		ShaderAttachment(shaderSource, GL_COMPUTE_SHADER)
	});
}

template<>
std::unique_ptr<Mesh> ResourceManager::CreateResource(const std::string& key)
{
	CachedMesh* cachedMesh = GetResourceFromContainer<CachedMesh>(_cachedMeshData, key);
	return _graphics->CreateMesh(key, cachedMesh);
}

template<>
std::unique_ptr<Font> ResourceManager::CreateResource(const std::string& key)
{
	return _graphics->CreateFont(key);
}

template<>
std::unique_ptr<Texture> ResourceManager::CreateResource(const std::string& key)
{
	std::vector<unsigned char> pixels;
	glm::ivec2 size;
	_filesystem->LoadTextureData(key, pixels, size);

	return _graphics->CreateTexture(key, pixels, size, GL_RGBA16, GL_NEAREST, GL_REPEAT);
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
	_cachedMeshData.Cleanup(includePersistent);
	_cachedPostEffect.Cleanup(includePersistent);
	_cachedSurfaceShader.Cleanup(includePersistent);
	_cachedTextData.Cleanup(includePersistent);
	_cachedTexture.Cleanup(includePersistent);
}

ResourceManager::ResourceManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{

}

ResourceManager::~ResourceManager()
{

}