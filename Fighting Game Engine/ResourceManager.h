#pragma once
#include "BaseService.h"
#include <string>
#include <memory>
#include <filesystem>
#include "JSON.h"
#include "ResourceWrapper.h"
#include "GameSceneManager.h"
#include "GameScene.h"
#include "ResourceContainer.h"

//TODO: Figure out a way to remove these and re-introduce the forward declarations
#include "Mesh.h"
#include "CachedMesh.h"
#include "SurfaceShader.h"
#include "ComputeShader.h"
#include "Material.h"
#include "Texture.h"
#include "Font.h"
#include "FilesystemManager.h"
#include "GraphicsGL.h"

////TODO: PUT BACK RIGHT NOW!!!
//class CachedMesh;
//class Mesh;
//class Texture;
//class Material;
//class SurfaceShader;
//class ComputeShader;
//class Font;
//class ShaderAttachment;

namespace fs = std::filesystem;

class Debug;
class GraphicsGL;
class FilesystemManager;

class ResourceManager : public BaseService
{
	VE_OBJECT_DECLARATION(ResourceManager);

private:
	ObjectReference<Debug> _debug;
	ObjectReference<GraphicsGL> _graphics;
	ObjectReference<FilesystemManager>_filesystem;
	ObjectReference<GameSceneManager> _gameSceneManager;

private:

#define ve_named_resource_container(ResourceType, ResourceName)\
	private:\
		ResourceContainer<std::string, ResourceType> _##ResourceName##Map;\
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

	ve_named_resource_container(std::string, TextData);
	ve_named_resource_container(json, JsonData);

#undef ve_named_resource_container
#undef ve_resource_container

protected:
	template<typename ResourceT>
	ResourceT CreateResource(const std::string& key);
	template<typename ResourceT>
	ResourceT* GetResourceFromContainer(ResourceContainer<std::string, ResourceT>& container, const std::string& key);
	template<typename ResourceT>
	void TrimContainer(ResourceContainer<std::string, ResourceT>& container);

	void PreprocessTextSource(std::string& inoutShaderSource);

	void HandleSceneLoaded(const GameScene* scene);

	void GenerateDefaultTextures();
	void LoadDefaultResources();

	void TrimContainers();
public:

	virtual void OnInit() override;
	void OnServiceInit() override;
	virtual void OnDestroyed() override;

	ResourceManager() = default;
	~ResourceManager() = default;
};

template<>
Texture ResourceManager::CreateResource(const std::string& key);
template<>
SurfaceShader ResourceManager::CreateResource(const std::string& key);
template<>
ComputeShader ResourceManager::CreateResource(const std::string& key);
template<>
Mesh ResourceManager::CreateResource(const std::string& key);
template<>
Font ResourceManager::CreateResource(const std::string& key);
template<>
std::string ResourceManager::CreateResource(const std::string& key);

template<class ResourceT>
ResourceT ResourceManager::CreateResource(const std::string& key)
{
	return _filesystem->GetFileResource<ResourceT>(key);
}

template<class ResourceT>
ResourceT* ResourceManager::GetResourceFromContainer(ResourceContainer<std::string, ResourceT>& container, const std::string& key)
{
	GameScene* curScene = _gameSceneManager->currentScene();
	std::string curSceneName = (curScene == nullptr) ? "" : curScene->dataPath().string();

	ResourceT* existingResource;
	if(container.TryGet(key, existingResource, curSceneName))
	{
		return existingResource;
	}

	fs::path path = key;
	bool isPersistent = (_gameSceneManager->currentScene() == nullptr) || (path.parent_path().stem().string() == "Base");

	return &container.Add(key, CreateResource<ResourceT>(key), isPersistent, curSceneName);
}

template <typename ResourceT>
void ResourceManager::TrimContainer(ResourceContainer<std::string, ResourceT>& container)
{
	if(_gameSceneManager.IsValid() && _gameSceneManager->currentScene() != nullptr)
	{
		const std::string& currentSceneName = _gameSceneManager->currentScene()->dataPath().string();
		container.Cleanup(
			[&](const ResourceWrapper<ResourceT>& resource) -> bool
			{
				return !resource.IsReferencedInScene(currentSceneName);
			});
	}
	else
	{
		container.Cleanup();
	}
}
