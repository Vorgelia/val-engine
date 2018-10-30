#pragma once
#include "BaseService.h"
#include <string>
#include <memory>
#include <filesystem>
#include "JSON.h"
#include "ResourceWrapper.h"
#include "GameSceneManager.h"
#include "GameScene.h"

////TODO: Figure out a way to remove these and re-introduce the forward declarations
//#include "Mesh.h"
//#include "CachedMesh.h"
//#include "SurfaceShader.h"
//#include "ComputeShader.h"
//#include "Material.h"
//#include "Texture.h"
//#include "PostEffect.h"
//#include "Font.h"
//#include "FilesystemManager.h"
//#include "GraphicsGL.h"

//TODO: PUT BACK RIGHT NOW!!!
class CachedMesh;
class Mesh;
class Texture;
class Material;
class SurfaceShader;
class ComputeShader;
class PostEffect;
class Font;
class ShaderAttachment;

namespace fs = std::filesystem;

class Debug;
class GraphicsGL;
class FilesystemManager;

class ResourceManager : public BaseService
{
private:
	Debug* _debug;
	GraphicsGL* _graphics;
	FilesystemManager* _filesystem;
	GameSceneManager* _gameSceneManager;

private:
	template <typename ResourceT>
	using ResourceContainer = std::unordered_map<std::string, ResourceWrapper<ResourceT>>;

#define ve_named_resource_container(ResourceType, ResourceName)\
	private:\
		ResourceContainer<ResourceType> _##ResourceName##Map;\
	public:\
		ResourceType* Get##ResourceName(const std::string& key) { return GetResourceFromContainer<ResourceType>(_##ResourceName##Map, key); }\
	private:
#define ve_resource_container(ResourceType) ve_named_resource_container(ResourceType, ResourceType)

	ve_resource_container(Mesh);
	ve_resource_container(CachedMesh);
	ve_resource_container(Texture);
	ve_resource_container(Material);
	ve_resource_container(SurfaceShader);
	ve_resource_container(ComputeShader);
	ve_resource_container(Font);
	ve_resource_container(PostEffect);

	ve_named_resource_container(std::string, TextData);
	ve_named_resource_container(json, JsonData);

#undef ve_named_resource_container
#undef ve_resource_container

protected:
	template<typename ResourceT>
	std::unique_ptr<ResourceT> CreateResource(const std::string& key);
	template<typename ResourceT>
	ResourceT* GetResourceFromContainer(ResourceContainer<ResourceT>& container, const std::string& key);
	template<typename ResourceT>
	void TrimContainer(ResourceContainer<ResourceT>& container);

	void PreprocessTextSource(std::string& inoutShaderSource);

	void HandleSceneLoaded(const GameScene* scene);

	void GenerateDefaultTextures();
	void LoadDefaultResources();

	void TrimContainers();
public:

	virtual void OnInit() override;
	virtual void OnDestroyed() override;

	ResourceManager() = default;
	~ResourceManager() = default;
};

template<>
std::unique_ptr<Texture> ResourceManager::CreateResource(const std::string& key);
template<>
std::unique_ptr<SurfaceShader> ResourceManager::CreateResource(const std::string& key);
template<>
std::unique_ptr<ComputeShader> ResourceManager::CreateResource(const std::string& key);
template<>
std::unique_ptr<Mesh> ResourceManager::CreateResource(const std::string& key);
template<>
std::unique_ptr<Font> ResourceManager::CreateResource(const std::string& key);
template<>
std::unique_ptr<std::string> ResourceManager::CreateResource(const std::string& key);

template<class ResourceT>
std::unique_ptr<ResourceT> ResourceManager::CreateResource(const std::string& key)
{
	return _filesystem->LoadFileResource<ResourceT>(key);
}

template<class ResourceT>
ResourceT* ResourceManager::GetResourceFromContainer(ResourceContainer<ResourceT>& container, const std::string& key)
{
	auto& iter = container.find(key);
	if(iter != container.end())
	{
		if(_gameSceneManager->currentScene() != nullptr)
		{
			iter->second._referencedLevels.emplace(_gameSceneManager->currentScene());
		}
		return iter->second.Get();
	}

	fs::path path = key;

	ResourceWrapper<ResourceT> resource = ResourceWrapper<ResourceT>(
		std::move(CreateResource<ResourceT>(key)),
		_gameSceneManager->currentScene() == nullptr || path.parent_path().stem().string() == "Base");
	if(!resource._isPersistent)
	{
		resource._referencedLevels.emplace(_gameSceneManager->currentScene()->name());
	}

	return container.emplace(key, std::move(resource)).first->second.Get();
}

template <typename ResourceT>
void ResourceManager::TrimContainer(ResourceContainer<ResourceT>& container)
{
	std::vector<std::string> membersToErase;
	for(auto& iter : container)
	{
		const ResourceWrapper<ResourceT>& resource = iter.second;
		if(resource._isPersistent)
		{
			continue;
		}

		if((_gameSceneManager->currentScene() == nullptr)
			|| (resource._referencedLevels.count(_gameSceneManager->currentScene()->name()) == 0))
		{
			membersToErase.emplace_back(iter.first);
		}
	}

	for(auto& iter : membersToErase)
	{
		container.erase(iter);
	}
}
