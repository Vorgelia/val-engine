#pragma once
#include <memory>
#include <string>
#include <vector>
#include <unordered_set>

template<typename ResourceT>
class ResourceWrapper
{
	friend class ResourceManager;

protected:
	std::unique_ptr<ResourceT> _resource;
	std::unordered_set<std::string> _referencedLevels;
	bool _isPersistent;

public:
	ResourceT* Get() { return _resource.get(); }

	ResourceWrapper(std::unique_ptr<ResourceT>&& resourcePtr, bool isPersistent = false);
	ResourceWrapper(ResourceWrapper<ResourceT>&& other) noexcept;
	ResourceWrapper(ResourceWrapper<ResourceT>& other) = delete;
	~ResourceWrapper() = default;
};

template <typename ResourceT>
ResourceWrapper<ResourceT>::ResourceWrapper(std::unique_ptr<ResourceT>&& resourcePtr, bool isPersistent)
	: _resource(std::move(resourcePtr))
	, _isPersistent(isPersistent)
{
	
}

template <typename ResourceT>
ResourceWrapper<ResourceT>::ResourceWrapper(ResourceWrapper<ResourceT>&& other) noexcept
{
	_resource = std::move(other._resource);
	_isPersistent = other._isPersistent;
}
