#pragma once
#include "BaseService.h"
#include <string>
#include <memory>
#include <boost/filesystem.hpp>
#include "JSON.h"
#include "ResourceContainer.h"

//TODO: Figure out a way to remove these and re-introduce the forward declarations
#include "Mesh.h"
#include "CachedMesh.h"
#include "SurfaceShader.h"
#include "ComputeShader.h"
#include "Material.h"
#include "Texture.h"
#include "PostEffect.h"
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
//class PostEffect;
//class Font;
//class ShaderAttachment;

namespace FS = boost::filesystem;

class Debug;
class GraphicsGL;
class FilesystemManager;

#define ve_named_resource_container(ResourceType, ResourceName)\
	private:\
		ResourceContainer<std::string, ResourceType> _cached##ResourceName;\
	public:\
		ResourceType* Get##ResourceName(const std::string& key) { return GetResourceFromContainer<ResourceType>(_cached##ResourceName, key); }\
	private:
#define ve_resource_container(ResourceType) ve_named_resource_container(ResourceType, ResourceType)


class ResourceManager : public BaseService
{
private:
	Debug* _debug;
	GraphicsGL* _graphics;
	FilesystemManager* _filesystem;

private:
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

	void GenerateDefaultTextures();
	void LoadDefaultResources();

	void PreprocessTextSource(std::string& inoutShaderSource);
	
public:
	template<typename ResourceT>
	std::unique_ptr<ResourceT> CreateResource(const std::string& key);

	template<typename ResourceT>
	ResourceT* GetResourceFromContainer(ResourceContainer<std::string, ResourceT>& container, const std::string& key);

	void Init() override;
	void Update() override;
	void Cleanup() override;

	void Unload(bool includePersistent = false);

	ResourceManager(GameInstance* serviceManager);
	~ResourceManager();
};

#undef VE_RESOURCE_GETTER

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
ResourceT* ResourceManager::GetResourceFromContainer(ResourceContainer<std::string, ResourceT>& container, const std::string& key)
{
	ResourceT* resource;
	if(container.TryGet(key, resource))
	{
		return resource;
	}

	FS::path path = key;
	return container.Add(
		key,
		std::move(CreateResource<ResourceT>(key)),
		path.parent_path().leaf().string() == "Base");
}