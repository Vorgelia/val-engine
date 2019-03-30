#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

template<typename ResourceT>
class ResourceWrapper
{

protected:
	std::unique_ptr<ResourceT> _resource;
	std::unordered_set<std::string> _referencedLevels;
	bool _isPersistent;

public:
	ResourceT& get() { return *_resource.get(); }
	bool isPersistent() const { return _isPersistent; }

	bool IsReferencedInScene(const std::string& sceneName) const;

	void RegisterSceneReference(const std::string& sceneName) { _referencedLevels.emplace(sceneName); }
	void UnregisterSceneReference(const std::string& sceneName) { _referencedLevels.erase(sceneName); }

	ResourceWrapper(ResourceT&& resource, bool isPersistent = false);
	ResourceWrapper(ResourceWrapper<ResourceT>&& other) noexcept;
	ResourceWrapper(ResourceWrapper<ResourceT>& other) = delete;
	~ResourceWrapper() = default;
};

template <typename ResourceT>
bool ResourceWrapper<ResourceT>::IsReferencedInScene(const std::string& sceneName) const
{
	return _referencedLevels.count(sceneName) != 0;
}

template <typename ResourceT>
ResourceWrapper<ResourceT>::ResourceWrapper(ResourceT&& resource, bool isPersistent)
	: _resource(std::make_unique<ResourceT>(std::move(resource)))
	, _isPersistent(isPersistent)
{
	
}

template <typename ResourceT>
ResourceWrapper<ResourceT>::ResourceWrapper(ResourceWrapper<ResourceT>&& other) noexcept
{
	_resource.swap(other._resource);
	_isPersistent = other._isPersistent;
	_referencedLevels = std::move(other._referencedLevels);
}
