#pragma once
#include "BaseService.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <boost\filesystem.hpp>
#include "ResourceContainer.h"
#include "JSON.h"
namespace FS = boost::filesystem;

class CachedMesh;
class Mesh;
class Texture;
class Material;
class SurfaceShader;
class ComputeShader;
class PostEffect;
class Font;

class Debug;
class GraphicsGL;
class FilesystemManager;

#define VE_NAMED_RESOURCE_CONTAINER(ResourceType, ResourceName)\
	private:\
		ResourceContainer<std::string, ResourceType> _cached##ResourceName;\
	public:\
		ResourceType* Get##ResourceName(const std::string& key) { return GetResourceFromContainer<ResourceType>(_cached##ResourceName, key); }\
	private:
#define VE_RESOURCE_CONTAINER(ResourceType) VE_NAMED_RESOURCE_CONTAINER(ResourceType, ResourceType)


class ResourceManager : public BaseService
{
private:
	Debug* _debug;
	GraphicsGL* _graphics;
	FilesystemManager* _filesystem;

private:
	VE_RESOURCE_CONTAINER(Mesh);
	VE_RESOURCE_CONTAINER(Texture);
	VE_RESOURCE_CONTAINER(Material);
	VE_RESOURCE_CONTAINER(SurfaceShader);
	VE_RESOURCE_CONTAINER(ComputeShader);
	VE_RESOURCE_CONTAINER(Font);
	VE_RESOURCE_CONTAINER(PostEffect);

	VE_NAMED_RESOURCE_CONTAINER(std::string, TextData);
	VE_NAMED_RESOURCE_CONTAINER(json, JsonData);

	ResourceContainer<std::string, CachedMesh> _cachedMeshData;

	void GenerateDefaultTextures();
	void LoadDefaultResources();

	void PreprocessShaderSource(std::string& inoutShaderSource);
	
public:
	template<typename ResourceT>
	std::unique_ptr<ResourceT> CreateResource(const std::string& key);
	template<>
	std::unique_ptr<SurfaceShader> CreateResource(const std::string& key);
	template<>
	std::unique_ptr<ComputeShader> CreateResource(const std::string& key);
	template<>
	std::unique_ptr<Mesh> CreateResource(const std::string& key);

	template<typename ResourceT>
	ResourceT* GetResourceFromContainer(ResourceContainer<std::string, ResourceT>& container, const std::string& key);

	void Init() override;
	void Update() override;
	void Cleanup() override;

	void Unload(bool includePersistent = false);

	ResourceManager(ServiceManager* serviceManager);
	~ResourceManager();
};

#undef VE_RESOURCE_GETTER

template<typename ResourceT>
inline std::unique_ptr<ResourceT> ResourceManager::CreateResource(const std::string& key)
{
	return _filesystem->LoadFileResource<ResourceT>(key);
	return nullptr;
}

template<typename ResourceT>
inline ResourceT* ResourceManager::GetResourceFromContainer(ResourceContainer<std::string, ResourceT>& container, const std::string& key)
{
	ResourceT* resource;
	if(container.TryGet(key, resource))
	{
		return resource;
	}

	FS::path path(key);

	if(!FS::exists(path))
	{
		return container.Add(key, nullptr);
	}

	return nullptr;
	
	return container.Add(
		key,
		std::move(CreateResource<ResourceT>(key)),
		path.parent_path().leaf().string() == "Base");
}
