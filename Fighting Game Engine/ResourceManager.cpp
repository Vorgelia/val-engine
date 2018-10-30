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

	_TextureMap.emplace("base_texture", ResourceWrapper<Texture>(_graphics->CreateTexture("base_texture", pixels, glm::ivec2(2, 2)), true));

	pixels = std::vector<unsigned char>{ 0, 0, 0, 255 };
	_TextureMap.emplace("black", ResourceWrapper<Texture>(_graphics->CreateTexture("black", pixels, glm::ivec2(1, 1)), true));

	pixels = std::vector<unsigned char>{ 255, 255, 255, 255 };
	_TextureMap.emplace("white", ResourceWrapper<Texture>(_graphics->CreateTexture("white", pixels, glm::ivec2(1, 1)), true));
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

void ResourceManager::HandleSceneLoaded(const GameScene* scene)
{
	TrimContainers();
}

void ResourceManager::OnInit()
{
	_debug = _owningInstance->Debug();
	_graphics = _owningInstance->Graphics();
	_filesystem = _owningInstance->Filesystem();
	_gameSceneManager = _owningInstance->GameSceneManager();

	_gameSceneManager->SceneLoaded += VE_DELEGATE_FUNC(GameSceneManager::GameSceneEventHandler, HandleSceneLoaded);

	GenerateDefaultTextures();
	LoadDefaultResources();
}

void ResourceManager::OnDestroyed()
{
	TrimContainers();
}

void ResourceManager::TrimContainers()
{
	TrimContainer(_ComputeShaderMap);
	TrimContainer(_FontMap);
	TrimContainer(_JsonDataMap);
	TrimContainer(_MaterialMap);
	TrimContainer(_MeshMap);
	TrimContainer(_CachedMeshMap);
	TrimContainer(_PostEffectMap);
	TrimContainer(_SurfaceShaderMap);
	TrimContainer(_TextDataMap);
	TrimContainer(_TextureMap);
}