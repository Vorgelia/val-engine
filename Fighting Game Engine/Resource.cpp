#include "Resource.h"
#include "ServiceManager.h"
#include "CachedMesh.h"
#include "Mesh.h"
#include "Shader.h"
#include "ResourceLoader.h"
#include "Material.h"
#include "Texture.h"
#include "PostEffect.h"
#include "Font.h"
#include "ResourceInitializer.h"
#include "DebugLog.h"
#include "GLStateTrack.h"
#include <memory>

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
	baseTextures.emplace(std::make_pair("base_texture", _graphics->CreateTexture("base_texture", pixels, glm::ivec2(2, 2), GL_RGBA, GL_NEAREST, GL_REPEAT)));

	pixels = std::vector<unsigned char>{ 0, 0, 0, 255 };
	baseTextures.emplace(std::make_pair("black", std::make_unique<Texture>("black", pixels, glm::ivec2(1, 1), GL_RGBA, GL_NEAREST, GL_REPEAT)));

	pixels = std::vector<unsigned char>{ 255, 255, 255, 255 };
	baseTextures.emplace(std::make_pair("white", std::make_unique<Texture>("white", pixels, glm::ivec2(1, 1), GL_RGBA, GL_NEAREST, GL_REPEAT)));
}

void ResourceManager::LoadDefaultResources()
{
	//Base shaders
	GetShader("Shaders/Base/2D");
	GetShader("Shaders/Base/Screen");
	//Base meshes
	GetMesh("Meshes/Base/quad.vm", true);
	GetMesh("Meshes/Base/screenQuad.vm", true);
	//Base materials
	GetMaterial("Materials/Base/Screen.vmat");
	//Fonts
	GetFont("Fonts/Amble.ttf");
}

void ResourceManager::Update() {}

Font* ResourceManager::GetFont(FS::path path)
{
	std::string pathString = path.string();
	auto& iter = fonts.find(pathString);

	if(iter != fonts.end())
	{
		return iter->second.get();
	}

	if(!FS::exists(path))
	{
		fonts.emplace(std::make_pair(pathString, nullptr));
		_debug->VE_LOG("Failed to load Font " + pathString);
		return nullptr;
	}

	return fonts.emplace(
		std::make_pair(pathString, std::make_unique<Font>(path))
	).first->second.get();
}

//The way these functions work is similar. If the resource exists in the per GameState resources, retrieve a pointer to it.
//Else, if the resource exists in the base resources, do the same.
//Else, attempt to load the resource, caching it as nullptr if it doesn't exist, so no further loading attempts are made.
Texture* ResourceManager::GetTexture(FS::path path)
{
	std::string pathString = path.string();

	auto& iter = textures.find(pathString);
	if(iter != textures.end())
	{
		return iter->second.get();
	}

	iter = baseTextures.find(pathString);
	if(iter != baseTextures.end())
	{
		return iter->second.get();
	}

	if(!FS::exists(path))
	{
		_debug->VE_LOG("Unable to find texture at " + pathString);
		textures.emplace(std::make_pair(pathString, nullptr));
		return nullptr;
	}

	auto* container = &textures;

	if(path.parent_path().leaf().string() == "Base")
	{
		container = &baseTextures;
	}

	std::vector<unsigned char> pixels;
	glm::ivec2 size;
	ResourceLoader::LoadTexture(path, pixels, size);

	return container->emplace(
		std::make_pair(
			pathString,
			_graphics->CreateTexture(pathString, pixels, size, GL_RGBA, GL_NEAREST, GL_REPEAT))
	).first->second.get();
}

PostEffect* ResourceManager::GetPostEffect(FS::path path)
{
	std::string pathString = path.string();
	auto& iter = postEffects.find(pathString);

	if(iter != postEffects.end())
	{
		return iter->second.get();
	}

	if(!FS::exists(path))
	{
		_debug->VE_LOG("Failed to load Texture " + pathString);
		postEffects.emplace(std::make_pair(pathString, nullptr));
		return nullptr;
	}

	return postEffects.emplace(std::make_pair(
		pathString, std::make_unique<PostEffect>(pathString)
	)).first->second.get();
}

Shader* ResourceManager::GetShader(std::string name)
{
	auto& iter = shaders.find(name);
	if(iter != shaders.end())
	{
		return iter->second.get();
	}

	if(!FS::exists(name + ".vert") || !FS::exists(name + ".frag"))
	{
		shaders.emplace(std::pair<std::string, std::unique_ptr<Shader>>(name, nullptr));
		_debug->VE_LOG("Failed to load Shader " + name);
		return nullptr;
	}

	return shaders.emplace(std::pair<std::string, std::unique_ptr<Shader>>(
		name,
		std::make_unique<Shader>(name, std::vector<ShaderAttachment>{
		ShaderAttachment(ResourceLoader::ReturnFile(name + ".vert"), GL_VERTEX_SHADER),
			ShaderAttachment(ResourceLoader::ReturnFile(name + ".frag"), GL_FRAGMENT_SHADER)
	}))).first->second.get();
}

Material* ResourceManager::GetMaterial(FS::path path)
{
	std::string pathString = path.string();

	auto& iter = materials.find(pathString);
	if(iter != materials.end())
	{
		return iter->second.get();
	}

	iter = baseMaterials.find(pathString);
	if(iter != baseMaterials.end())
	{
		return iter->second.get();
	}

	if(!FS::exists(path))
	{
		materials.emplace(std::pair<std::string, std::unique_ptr<Material>>(pathString, nullptr));
		_debug->VE_LOG("Failed to load material " + pathString);
		return nullptr;
	}

	auto* container = &materials;

	if(path.parent_path().leaf().string() == "Base")
	{
		container = &baseMaterials;
	}

	return container->emplace(std::pair<std::string, std::unique_ptr<Material>>(
		pathString,
		std::make_unique<Material>(pathString))
	).first->second.get();
}

Material* ResourceManager::CopyMaterial(Material* mat)
{
	if(mat == nullptr)
		return nullptr;

	int copyIndex = 0;
	while(materials.count(mat->name + "_Copy" + std::to_string(copyIndex)) > 0)
	{
		++copyIndex;
	}

	auto& iter =
		materials.emplace(std::pair<std::string, std::unique_ptr<Material>>(
			mat->name + "_Copy" + std::to_string(copyIndex),
			std::make_unique<Material>(*mat)))
		.first;
	iter->second->name = mat->name + "_Copy" + std::to_string(copyIndex);
	return iter->second.get();
}

Mesh* ResourceManager::GetMesh(FS::path path, bool editable)
{
	std::string pathString = path.string();

	auto& iter = meshes.find(pathString);
	if(iter != meshes.end())
	{
		return iter->second.get();
	}

	iter = baseMeshes.find(pathString);
	if(iter != baseMeshes.end())
	{
		return iter->second.get();
	}

	if(!FS::exists(path))
	{
		_debug->VE_LOG("Failed to load Mesh: " + pathString);
		postEffects.emplace(std::pair<std::string, std::unique_ptr<PostEffect>>(pathString, nullptr));
		return nullptr;
	}

	auto& cachedMeshIter = cachedMeshes.find(pathString);

	if(cachedMeshIter == cachedMeshes.end())
	{
		cachedMeshIter = cachedMeshes.emplace(
			std::pair<std::string, std::unique_ptr<CachedMesh>>
			(pathString, std::make_unique<CachedMesh>(path)))
			.first;
	}

	iter = meshes.emplace(
		std::pair<std::string, std::unique_ptr<Mesh>>
		(pathString, std::make_unique<Mesh>(pathString, cachedMeshIter->second.get(), editable)))
		.first;

	if(!iter->second->valid())
	{
		_debug->VE_LOG("Imported invalid Mesh: " + pathString);
	}

	return iter->second.get();
}

void ResourceManager::Unload()
{
	meshes.clear();
	cachedMeshes.clear();

	for(auto& tex : textures)
	{
		_graphics->DestroyTexture(*(tex.second.get()));
	}
	textures.clear();

	materials.clear();
}

ResourceManager::ResourceManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{
	_debug = serviceManager->Debug();
	_graphics = _serviceManager->Graphics();

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

ResourceManager::~ResourceManager()
{
	baseMeshes.clear();

	Unload();

	baseMaterials.clear();
	shaders.clear();

	for(auto& tex : baseTextures)
	{
		_graphics->DestroyTexture(*(tex.second.get()));
	}
	baseTextures.clear();

	fonts.clear();
	postEffects.clear();
}