#pragma once
#include <string>
#include "JSON.h"
#include "ReflectionTraits.h"

class IReflectable;
class BaseObject;
class BaseSerializationProxy;

class BaseReflectionField
{
	friend class BaseSerializationProxy;
public:
	const std::string name;
	const ReflectionFieldType type;

	virtual void* data() const = 0;

	virtual void DeserializeProxy(BaseSerializationProxy& proxy) = 0;
	virtual void SerializeProxy(BaseSerializationProxy& proxy) = 0;

	BaseReflectionField(std::string name, ReflectionFieldType type)
		: name(std::move(name)), type(type) { }

	virtual ~BaseReflectionField() = default;
};

template<typename ValueT>
class ReflectionField : public BaseReflectionField
{
	friend class BaseSerializationProxy;
protected:
	ValueT* const _data;

public:
	void* data() const override { return _data; }

	virtual void DeserializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.Get<ValueT>(name, *_data);
	}

	virtual void SerializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.Set<ValueT>(name, *_data);
	}

	ReflectionField(std::string name, ValueT* data)
		: BaseReflectionField(std::move(name), ReflectionFieldTraits<ve::remove_object_ptr_t<ValueT>>::field_type)
		, _data(data)
	{
	}
};

template<typename ValueT>
class ObjectReflectionField : public BaseReflectionField
{
	friend class BaseSerializationProxy;
protected:
	ve::unique_object_ptr<ValueT>* const _data;
	BaseObject* _outer;

public:
	void* data() const override { return _data; }

	void DeserializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.GetObject<ValueT>(name, *_data, _outer);
	}

	void SerializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.Set<ve::unique_object_ptr<ValueT>>(name, *_data);
	}

	ObjectReflectionField(std::string name, ve::unique_object_ptr<ValueT>* data, BaseObject* outer)
		: BaseReflectionField(std::move(name), ReflectionFieldTraits<ve::remove_object_ptr_t<ValueT>>::field_type)
		, _data(data)
		, _outer(outer)
	{
	}
};

template<typename ValueT>
class ArrayReflectionField : public BaseReflectionField
{
	friend class BaseSerializationProxy;
protected:
	std::vector<ValueT>* const _data;

public:
	void* data() const override { return _data; }

	void DeserializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.GetArray<ValueT>(name, *_data);
	}

	void SerializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.SetArray<ValueT>(name, *_data);
	}

	ArrayReflectionField(std::string name, std::vector<ValueT>* data)
		: BaseReflectionField(std::move(name), ReflectionFieldType::Array)
		, _data(data)
	{
	}
};

template<typename ValueT>
class ObjectArrayReflectionField : public BaseReflectionField
{
	friend class BaseSerializationProxy;
protected:
	std::vector<ValueT>* const _data;
	BaseObject* _outer;

public:
	void* data() const override { return _data; }

	void DeserializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.FillObjectArray<ValueT>(name, *_data, _outer);
	}

	void SerializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.SetArray<ValueT>(name, *_data);
	}

	ObjectArrayReflectionField(std::string name, std::vector<ValueT>* data, BaseObject* outer)
		: BaseReflectionField(std::move(name), ReflectionFieldType::Array)
		, _data(data)
		, _outer(outer)
	{
	}
};

template<typename KeyT, typename ValueT>
class MapReflectionField : public BaseReflectionField
{
	friend class BaseSerializationProxy;
protected:
	std::unordered_map<KeyT, ValueT>* const _data;

public:
	void* data() const override { return _data; }

	void DeserializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.GetMap<KeyT, ValueT>(name, *_data);
	}
	void SerializeProxy(BaseSerializationProxy& proxy) override
	{
		proxy.SetMap<KeyT, ValueT>(name, *_data);
	}

	MapReflectionField(std::string name, std::unordered_map<KeyT, ValueT>* data)
		: BaseReflectionField(std::move(name), ReflectionFieldType::Map)
		, _data(data)
	{
	}
};