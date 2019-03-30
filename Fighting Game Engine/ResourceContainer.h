#pragma once
#include <unordered_map>
#include <memory>
#include "ResourceWrapper.h"
#include <algorithm>

template<typename KeyT, typename ResourceT>
class ResourceContainer
{
	typedef std::unordered_map<KeyT, ResourceWrapper<ResourceT>> ContainerT;
protected:
	ContainerT _resources;

public:
	virtual ResourceT& Add(const KeyT& key, ResourceT&& value, bool isPersistent = false, const std::string& currentSceneName = "");
	virtual bool TryGet(const KeyT& key, ResourceT*& out_value, const std::string& currentSceneName = "");

	void Cleanup(bool includePersistent = false);
	void Cleanup(std::function<bool(const ResourceWrapper<ResourceT>&)> predicate, bool includePersistent = false);

	ResourceContainer() = default;
	virtual ~ResourceContainer() = default;
};

template<typename KeyT, typename ResourceT>
inline bool ResourceContainer<KeyT, ResourceT>::TryGet(const KeyT& key, ResourceT*& out_value, const std::string& currentSceneName)
{
	auto& iter = _resources.find(key);
	if(iter != _resources.end())
	{
		out_value = &iter->second.get();
		iter->second.RegisterSceneReference(currentSceneName);
		return true;
	}

	return false;
}

template<typename KeyT, typename ResourceT>
inline ResourceT& ResourceContainer<KeyT, ResourceT>::Add(const KeyT& key, ResourceT&& value, bool isPersistent, const std::string& currentSceneName)
{
	ResourceWrapper<ResourceT>& resource = _resources.emplace(std::make_pair(key, ResourceWrapper<ResourceT>(std::move(value), isPersistent))).first->second;
	if(!currentSceneName.empty())
	{
		resource.RegisterSceneReference(currentSceneName);
	}
	return resource.get();
}

template<typename KeyT, typename ResourceT>
inline void ResourceContainer<KeyT, ResourceT>::Cleanup(bool includePersistent)
{
	if(includePersistent)
	{
		_resources.clear();
		return;
	}
	Cleanup([](const ResourceWrapper<ResourceT>&) { return true; }, includePersistent);
}

template<typename KeyT, typename ResourceT>
inline void ResourceContainer<KeyT, ResourceT>::Cleanup(std::function<bool(const ResourceWrapper<ResourceT>&)> predicate, bool includePersistent)
{
	for(auto& iter = _resources.begin(); iter != _resources.end();)
	{
		if((includePersistent || !iter->second.isPersistent()) && predicate(iter->second))
		{
			_resources.erase(iter++);
		}
		else
		{
			++iter;
		}
	}
}