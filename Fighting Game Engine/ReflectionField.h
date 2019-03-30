#pragma once
#include <string>
#include "JSON.h"
#include "IReflectable.h"
#include "ReflectionTraits.h"

class IReflectable;

class BaseReflectionField
{
public:
	const std::string name;
	const ReflectionFieldType type;

	//TODO: Replace json with SerializationProxy
	virtual void Deserialize(const JSON::json_t& j) = 0;
	virtual json Serialize() = 0;

	BaseReflectionField(std::string name, ReflectionFieldType type)
		: name(std::move(name)), type(type) { }

	virtual ~BaseReflectionField() = default;
};

template<typename ValueT>
class ReflectionField : public BaseReflectionField
{
protected:
	ValueT* const _data;

public:
	virtual void Deserialize(const json& j) override
	{
		if constexpr(std::is_pointer_v<ValueT> && std::is_base_of_v<BaseObject, std::remove_pointer_t<ValueT>>)
		{
			BaseObject* objectPtr = static_cast<BaseObject*>(*_data);
			if(ve::IsValid(objectPtr))
			{
				objectPtr->Deserialize(j);
			}
		}
		else if constexpr(std::is_base_of_v<IReflectable, ValueT>)
		{
			reinterpret_cast<IReflectable*>(_data)->Deserialize(j);
		}
		else
		{
			*_data = JSON::Get<ValueT>(j);
		}
	}

	virtual json Serialize() override
	{
		if constexpr(std::is_pointer_v<ValueT> && std::is_base_of_v<BaseObject, std::remove_pointer_t<ValueT>>)
		{
			BaseObject* objectPtr = static_cast<BaseObject*>(*_data);
			return ve::IsValid(objectPtr) ? objectPtr->Serialize() : json();
		}
		else
		{
			return JSON::ToJson<ValueT>(*_data);
		}
	}

	ReflectionField(std::string name, ValueT* data)
		: BaseReflectionField(std::move(name), ReflectionFieldTraits<ValueT>::field_type)
		, _data(data)
	{
	}
};

class JsonReflectionField : public BaseReflectionField
{
protected:
	json* const _data;

public:
	virtual void Deserialize(const JSON::json_t& j) override
	{
		*_data = j;
	}

	virtual json Serialize() override
	{
		return *_data;
	}

	JsonReflectionField(std::string name, json* data)
		: BaseReflectionField(std::move(name), ReflectionFieldType::Json)
		, _data(data)
	{
	}
};

template<typename ValueT>
class ArrayReflectionField : public BaseReflectionField
{
protected:
	std::vector<ValueT>* const _data;

public:
	virtual void Deserialize(const json& j) override
	{
		_data->clear();
		for(const json& iter : j)
		{
			_data->emplace_back(JSON::Get<ValueT>(iter));
		}
	}
	virtual json Serialize() override
	{
		json j = json::array();
		for(const ValueT& iter : *_data)
		{
			j.emplace_back(JSON::ToJson<ValueT>(iter));
		}

		return j;
	}
	
	ArrayReflectionField(std::string name, std::vector<ValueT>* data)
		: BaseReflectionField(std::move(name), ReflectionFieldType::Array)
		, _data(data)
	{
	}
};

template<typename KeyT, typename ValueT>
class MapReflectionField : public BaseReflectionField
{
protected:
	std::unordered_map<KeyT, ValueT>* const _data;

public:
	virtual void Deserialize(const json& j) override
	{
		_data->clear();
		for(auto iter = j.begin() ; iter != j.end() ; ++iter)
		{
			_data->emplace(JSON::Get<KeyT>(iter.key()), JSON::Get<ValueT>(iter.value()));
		}
	}
	virtual json Serialize() override
	{
		json j = {};
		for(auto& iter : *_data)
		{
			j.emplace(JSON::ToJson<KeyT>(iter.first), JSON::ToJson<ValueT>(iter.second));
		}
		
		return j;
	}

	MapReflectionField(std::string name, std::unordered_map<KeyT, ValueT>* data)
		: BaseReflectionField(std::move(name), ReflectionFieldType::Map)
		, _data(data)
	{
	}
};

class LambdaReflectionField : public BaseReflectionField
{
protected:
	std::function<void(const json&)> _deserializeLambda;
	std::function<json()> _serializeLambda;

public:
	virtual void Deserialize(const JSON::json_t& j) override
	{
		_deserializeLambda(j);
	}

	virtual json Serialize() override
	{
		return _serializeLambda();
	}

	LambdaReflectionField(std::string name, std::function<void(const json&)> deserializeLambda, std::function<json()> serializeLambda)
		: BaseReflectionField(name, ReflectionFieldType::Lambda)
		, _serializeLambda(std::move(serializeLambda))
		, _deserializeLambda(std::move(deserializeLambda))
	{
		
	}
};