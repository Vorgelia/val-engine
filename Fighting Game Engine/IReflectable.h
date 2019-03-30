#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <utility>
#include "JSON.h"
#include "TemplateUtils.h"
#include "ReflectionField.h"

#define VE_REFLECTION_ARG(param) #param, param
#define VE_PRIVATE_REFLECTION_ARG(param) #param, _##param

#define VE_REFLECTION_VAR(type, param) AddReflection##type(#param, param)
#define VE_PRIVATE_REFLECTION_VAR(type, param) AddReflection##type(#param, _##param)

class IReflectable
{
private:
	mutable std::unordered_map<std::string, std::shared_ptr<BaseReflectionField>> _storedFields;

protected:
	template<typename T>
	void AddReflectionField(const std::string& name, const T& field) const;

	void AddReflectionLambdaField(const std::string& name, std::function<void(const json&)> deserializeLambda, std::function<json()> serializeLambda);
	void AddReflectionJsonField(const std::string& name, const json& field) const;

	template<typename ValueT>
	void AddReflectionArray(const std::string& name, const std::vector<ValueT>& field) const;
	template<typename KeyT, typename ValueT>
	void AddReflectionMap(const std::string& name, const std::unordered_map<KeyT, ValueT>& field) const;

	virtual void RegisterReflectionFields() const {};

public:
	virtual json Serialize() const;
	virtual void Deserialize(const json& j);

	bool TrySerializeField(const std::string& name, json& out_json) const;
	bool TryDeserializeField(const std::string& name, const json& j);

	IReflectable() = default;
	virtual ~IReflectable() = default;
};

template <typename T>
void IReflectable::AddReflectionField(const std::string& name, const T& field) const
{
	if constexpr(std::is_same_v<T, json>)
	{
		AddReflectionJsonField(name, field);
	}
	else
	{
		_storedFields.insert_or_assign(name, std::make_shared<ReflectionField<T>>(name, const_cast<T*>(&field)));
	}
}

inline void IReflectable::AddReflectionLambdaField(const std::string& name, std::function<void(const json&)> deserializeLambda, std::function<json()> serializeLambda)
{
	_storedFields.insert_or_assign(name, std::make_shared<LambdaReflectionField>(name, deserializeLambda, serializeLambda));
}

inline void IReflectable::AddReflectionJsonField(const std::string& name, const json& field) const
{
	_storedFields.insert_or_assign(name, std::make_shared<JsonReflectionField>(name, const_cast<json*>(&field)));
}

template <typename ValueT>
void IReflectable::AddReflectionArray(const std::string& name, const std::vector<ValueT>& field) const
{
	_storedFields.insert_or_assign(name, std::make_shared<ArrayReflectionField<ValueT>>(name, const_cast<std::vector<ValueT>*>(&field)));
}

template <typename KeyT, typename ValueT>
void IReflectable::AddReflectionMap(const std::string& name, const std::unordered_map<KeyT, ValueT>& field) const
{
	_storedFields.insert_or_assign(name, std::make_shared<MapReflectionField<KeyT, ValueT>>(name, const_cast<std::unordered_map<KeyT, ValueT>*>(&field)));
}

inline json IReflectable::Serialize() const
{
	if(_storedFields.empty())
	{
		RegisterReflectionFields();
	}

	json j;
	for(auto& iter : _storedFields)
	{
		j.emplace(iter.first, iter.second->Serialize());
	}

	return j;
}

inline void IReflectable::Deserialize(const json& j)
{
	if(_storedFields.empty())
	{
		RegisterReflectionFields();
	}

	std::string str = j.dump();
	for(auto& iter : _storedFields)
	{
		json fieldJson;
		if(JSON::TryGetMember(j, iter.first, fieldJson))
		{
			std::string str2 = fieldJson.dump();
			iter.second->Deserialize(fieldJson);
		}
	}
}

inline bool IReflectable::TrySerializeField(const std::string& name, json& out_json) const
{
	auto& iter = _storedFields.find(name);
	if(iter == _storedFields.end())
	{
		return false;
	}

	out_json = iter->second->Serialize();
	return true;
}

inline bool IReflectable::TryDeserializeField(const std::string& name, const json& j)
{
	auto& iter = _storedFields.find(name);
	if(iter == _storedFields.end())
	{
		return false;
	}

	iter->second->Deserialize(j);
	return true;
}