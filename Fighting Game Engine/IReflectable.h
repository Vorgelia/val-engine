#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <utility>
#include "TemplateUtils.h"
#include "JSON.h"
#include "ReflectionField.h"

//TODO: Cleanup all of this with if constexpr
class IReflectable
{
private:
	std::unordered_map<std::string, std::unique_ptr<BaseReflectionField>> _storedFields;

protected:
	template<typename T>
	void AddReflectionField(const std::string& name, const T& field);
	template<typename ValueT>
	void AddReflectionArray(const std::string& name, const std::vector<ValueT>& field);
	template<typename KeyT, typename ValueT>
	void AddReflectionMap(const std::string& name, const std::unordered_map<KeyT, ValueT>& field);

public:
	virtual json Serialize() const;
	virtual void Deserialize(const json& j);

	bool TrySerializeField(const std::string& name, json& out_json) const;
	bool TryDeserializeField(const std::string& name, const json& j);

	IReflectable() = default;
	virtual ~IReflectable() = default;
};

template <typename T>
void IReflectable::AddReflectionField(const std::string& name, const T& field)
{
	_storedFields.insert_or_assign(name, std::make_unique<ReflectionField<T>>(name, const_cast<T*>(&field)));
}

template <typename ValueT>
void IReflectable::AddReflectionArray(const std::string& name, const std::vector<ValueT>& field)
{
	_storedFields.insert_or_assign(name, std::make_unique<ArrayReflectionField<ValueT>>(name, const_cast<std::vector<ValueT>*>(&field)));
}

template <typename KeyT, typename ValueT>
void IReflectable::AddReflectionMap(const std::string& name, const std::unordered_map<KeyT, ValueT>& field)
{
	_storedFields.insert_or_assign(name, std::make_unique<MapReflectionField<KeyT, ValueT>>(name, const_cast<std::unordered_map<KeyT, ValueT>*>(&field)));
}

inline json IReflectable::Serialize() const
{
	json j;
	for(auto& iter : _storedFields)
	{
		j.emplace(iter.first, iter.second->Serialize());
	}

	return j;
}

inline void IReflectable::Deserialize(const json& j)
{
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
