#pragma once
#include <unordered_map>
#include <memory>

template<typename KeyT, typename ResourceT>
class ResourceContainer
{
	typedef std::unordered_map<KeyT, std::unique_ptr<ResourceT>> ContainerT;
protected:
	ContainerT _persistentResources;
	ContainerT _resources;

public:
	virtual ResourceT* Add(const KeyT& key, std::unique_ptr<ResourceT>& value, bool isPersistent = false);
	virtual bool TryGet(const KeyT& key, ResourceT*& out_value);

	void Cleanup(bool includePersistent = false);

	ResourceContainer(size_t capacityHint = 0);
	~ResourceContainer() = default;
};

template<typename KeyT, typename ResourceT>
inline bool ResourceContainer<KeyT, ResourceT>::TryGet(const KeyT& key, ResourceT*& out_value)
{
	auto& iter = _resources.find(key);
	if(iter != _resources.end())
	{
		out_value = iter->second.get();
		return true;
	}

	iter = _persistentResources.find(key);
	if(iter != _persistentResources.end())
	{
		out_value = iter->second.get();
		return true;
	}

	return false;
}

template<typename KeyT, typename ResourceT>
inline ResourceT * ResourceContainer<KeyT, ResourceT>::Add(const KeyT& key, std::unique_ptr<ResourceT>& value, bool isPersistent)
{
	ContainerT* container = isPersistent ? &_persistentResources : &_resources;
	return container->insert(
		std::make_pair(key, value)
	).first->second.get();
}

template<typename KeyT, typename ResourceT>
inline void ResourceContainer<KeyT, ResourceT>::Cleanup(bool includePersistent)
{
	_resources.clear();
	if(includePersistent)
	{
		_persistentResources.clear();
	}
}

template<typename KeyT, typename ResourceT>
inline ResourceContainer<KeyT, ResourceT>::ResourceContainer(size_t capacityHint)
{
	_persistentResources.reserve(capacityHint);
	_resources.reserve(capacityHint);
}
