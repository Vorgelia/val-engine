#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <utility>
#include "TemplateUtils.h"
#include "ReflectionField.h"
#include "ObjectPtr.h"

class BaseSerializationProxy;

#define VE_REFLECTION_ARG(param) #param, param
#define VE_PRIVATE_REFLECTION_ARG(param) #param, _##param

#define VE_REFLECTION_VAR(type, param, ...) AddReflection##type(#param, param, __VA_ARGS__)
#define VE_PRIVATE_REFLECTION_VAR(type, param, ...) AddReflection##type(#param, _##param, __VA_ARGS__)

class IReflectable
{
private:
	mutable bool _hasBeenInitialized = false;
	mutable std::unordered_map<std::string, std::shared_ptr<BaseReflectionField>> _storedFields;
	std::unordered_map<std::string, std::shared_ptr<BaseReflectionField>>& storedFields() const;

protected:
	template<typename T>
	void AddReflectionField(const std::string& name, const T& field) const;

	template<typename ValueT>
	void AddReflectionObject(const std::string& name, const ve::unique_object_ptr<ValueT>& field, const BaseObject* outer) const;

	template<typename ValueT>
	void AddReflectionArray(const std::string& name, const std::vector<ValueT>& field) const;

	template<typename ValueT>
	void AddReflectionObjectArray(const std::string& name, const std::vector<ValueT>& field, const BaseObject* outer) const;

	template<typename KeyT, typename ValueT>
	void AddReflectionMap(const std::string& name, const std::unordered_map<KeyT, ValueT>& field) const;

	virtual void RegisterReflectionFields() const {};

public:
	void SerializeProxy(BaseSerializationProxy& proxy) const;
	void DeserializeProxy(BaseSerializationProxy& proxy);

	virtual void OnSerialized(BaseSerializationProxy& proxy) const {}
	virtual void OnDeserialized(BaseSerializationProxy& proxy) {}

	IReflectable() = default;
	virtual ~IReflectable() = default;
};

inline std::unordered_map<std::string, std::shared_ptr<BaseReflectionField>>& IReflectable::storedFields() const
{
	if (!_hasBeenInitialized)
	{
		RegisterReflectionFields();
		_hasBeenInitialized = true;
	}

	return _storedFields;
}

template <typename T>
void IReflectable::AddReflectionField(const std::string& name, const T& field) const
{
	_storedFields.insert_or_assign(name, std::make_shared<ReflectionField<T>>(name, const_cast<T*>(&field)));
}

template <typename ValueT>
void IReflectable::AddReflectionObject(const std::string& name, const ve::unique_object_ptr<ValueT>& field, const BaseObject* outer) const
{
	_storedFields.insert_or_assign(name, std::make_shared<ObjectReflectionField<ValueT>>(name, const_cast<ve::unique_object_ptr<ValueT>*>(&field), const_cast<BaseObject*>(outer)));
}

template <typename ValueT>
void IReflectable::AddReflectionArray(const std::string& name, const std::vector<ValueT>& field) const
{
	_storedFields.insert_or_assign(name, std::make_shared<ArrayReflectionField<ValueT>>(name, const_cast<std::vector<ValueT>*>(&field)));
}

template <typename ValueT>
void IReflectable::AddReflectionObjectArray(const std::string& name, const std::vector<ValueT>& field, const BaseObject* outer) const
{
	_storedFields.insert_or_assign(name, std::make_shared<ObjectArrayReflectionField<ValueT>>(name, const_cast<std::vector<ValueT>*>(&field), const_cast<BaseObject*>(outer)));
}

template <typename KeyT, typename ValueT>
void IReflectable::AddReflectionMap(const std::string& name, const std::unordered_map<KeyT, ValueT>& field) const
{
	_storedFields.insert_or_assign(name, std::make_shared<MapReflectionField<KeyT, ValueT>>(name, const_cast<std::unordered_map<KeyT, ValueT>*>(&field)));
}

inline void IReflectable::SerializeProxy(BaseSerializationProxy& proxy) const
{
	for (auto& iter : storedFields())
	{
		iter.second->SerializeProxy(proxy);
	}

	OnSerialized(proxy);
}

inline void IReflectable::DeserializeProxy(BaseSerializationProxy& proxy)
{
	for (auto& iter : storedFields())
	{
		iter.second->DeserializeProxy(proxy);
	}

	OnDeserialized(proxy);
}