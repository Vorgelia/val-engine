#include "ResourceManager.h"
#include "GameInstance.h"
#include "FilesystemManager.h"
#include "GraphicsGL.h"
#include "DebugLog.h"

#include "Mesh.h"
#include "CachedMesh.h"
#include "SurfaceShader.h"
#include "ComputeShader.h"
#include "Material.h"
#include "Texture.h"
#include "Font.h"
#include "FilesystemManager.h"
#include "GraphicsGL.h"

#include <regex>
#include <boost/algorithm/string/erase.hpp>

VE_OBJECT_DEFINITION(ResourceManager)

void ResourceManager::GenerateDefaultTextures()
{
	std::vector<unsigned char>& pixels = std::vector<unsigned char>
	{
		255, 0, 255, 255,	 0, 0, 0, 255,
		0, 0, 0, 255,	 255, 0, 255, 255
	};

	_TextureMap.Add("base_texture", _graphics->CreateTexture("base_texture", pixels, glm::ivec2(2, 2)), true);

	pixels = std::vector<unsigned char>{ 0, 0, 0, 255 };
	_TextureMap.Add("black", _graphics->CreateTexture("black", pixels, glm::ivec2(1, 1)), true);

	pixels = std::vector<unsigned char>{ 255, 255, 255, 255 };
	_TextureMap.Add("white", _graphics->CreateTexture("white", pixels, glm::ivec2(1, 1)), true);
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
Texture ResourceManager::CreateResource(const std::string& key)
{
	glm::ivec2 textureSize;
	std::vector<unsigned char> textureData;
	_filesystem->LoadTextureData(key, textureData, textureSize);
	return _graphics->CreateTexture(key, textureData, textureSize);
}

template<>
SurfaceShader ResourceManager::CreateResource(const std::string& key)
{
	std::string vertSource = _filesystem->ReturnFile(key + ".vert");
	PreprocessTextSource(vertSource);
	std::string fragSource = _filesystem->ReturnFile(key + ".frag");
	PreprocessTextSource(fragSource);

	return _graphics->CreateShader<SurfaceShader>(
		key,
		std::vector<ShaderAttachment>{
		ShaderAttachment(vertSource, GL_VERTEX_SHADER),
			ShaderAttachment(fragSource, GL_FRAGMENT_SHADER)});
}

template<>
ComputeShader ResourceManager::CreateResource(const std::string& key)
{
	std::string shaderSource = _filesystem->ReturnFile(key + ".comp");
	PreprocessTextSource(shaderSource);

	return _graphics->CreateShader<ComputeShader>(
		key,
		std::vector<ShaderAttachment>{
			ShaderAttachment(shaderSource, GL_COMPUTE_SHADER)
		}
	);
}

template<>
inline Mesh ResourceManager::CreateResource(const std::string& key)
{
	CachedMesh* cachedMesh = GetCachedMesh(key);
	return _graphics->CreateMesh(key, cachedMesh);
}

template<>
inline Font ResourceManager::CreateResource(const std::string& key)
{
	return _graphics->CreateFont(key);
}

template<>
inline std::string ResourceManager::CreateResource(const std::string& key)
{
	std::string resource = _filesystem->GetFileResource<std::string>(key);
	PreprocessTextSource(resource);
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
}

void ResourceManager::OnServiceInit()
{
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
	TrimContainer(_SurfaceShaderMap);
	TrimContainer(_TextDataMap);
	TrimContainer(_TextureMap);
}