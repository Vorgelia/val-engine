#pragma once
#include "JSON.h"
#include "ObjectInitializer.h"
#include "EngineConfigData.h"
#include "ObjectPtr.h"
#include "SerializationProxy.h"

class GameInstance;
class GameScene;
class BaseObject;
class SceneObject;
class GameObject;
class ObjectComponent;
class BaseSerializationProxy;

class ObjectFactory
{
	typedef std::function<BaseObject* ()> BaseObjectGenerator;
	typedef std::unordered_map<std::string, BaseObjectGenerator> BaseObjectGeneratorMap;

protected:
	static BaseObjectGeneratorMap* objectGenerators();

	static const EngineConfigData& GetEngineConfigData(BaseObject* contextObject);
	static json* GetJsonResource(const std::string& name, BaseObject* contextObject);

public:
	template<typename ObjectT>
	static bool RegisterObjectGenerator(const std::string& name);

	template<typename ObjectT>
	static ve::unique_object_ptr<ObjectT> CreateObject(BaseObject* outer, BaseSerializationProxy* proxy = nullptr);
	template<typename ObjectT>
	static ve::unique_object_ptr<ObjectT> CreateObjectDeferred();

	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectOfClass(const std::string & className, BaseObject * outer, BaseSerializationProxy * proxy = nullptr);
	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectOfClassDeferred(const std::string & className);
	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectFromData(BaseObject * outer, BaseSerializationProxy & proxy);
	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectFromDataDeferred(BaseObject * outer, BaseSerializationProxy & proxy);

	static void InitializeObject(BaseObject* object, BaseObject* outer, BaseSerializationProxy* proxy = nullptr);

	ObjectFactory() = delete;
};

namespace ve = ValEngine;

template <typename ObjectT>
bool ObjectFactory::RegisterObjectGenerator(const std::string& name)
{
	objectGenerators()->emplace(name, []()->BaseObject * { return new ObjectT(); });
	return true;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObject(BaseObject* outer, BaseSerializationProxy* proxy)
{
	ve::unique_object_ptr<ObjectT> object = CreateObjectDeferred<ObjectT>();

	InitializeObject(object.get(), outer, proxy);

	return object;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectDeferred()
{
	static_assert(std::is_base_of_v<BaseObject, ObjectT>);

	return ve::unique_object_ptr<ObjectT> { new ObjectT() };
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectOfClass(const std::string& className, BaseObject* outer, BaseSerializationProxy* proxy)
{
	ve::unique_object_ptr<ObjectT> object = CreateObjectOfClassDeferred<ObjectT>(className);

	InitializeObject(object.get(), outer, proxy);

	return object;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectOfClassDeferred(const std::string& className)
{
	static_assert(std::is_base_of_v<BaseObject, ObjectT>);

	const auto* generators = objectGenerators();

	auto generatorIter = generators->find(className);
	if (generatorIter == generators->end())
	{
		__debugbreak();
		return nullptr;
	}

	ve::unique_object_ptr<ObjectT> object{ dynamic_cast<ObjectT*>(generatorIter->second()) };
	if (object.get() == nullptr)
	{
		__debugbreak();
		return nullptr;
	}

	return object;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectFromData(BaseObject* outer, BaseSerializationProxy& proxy)
{
	ve::unique_object_ptr<ObjectT> object = CreateObjectFromDataDeferred<ObjectT>(outer, proxy);

	InitializeObject(object.get(), outer, &proxy);

	return object;
}

template<typename ObjectT>
inline ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectFromDataDeferred(BaseObject* outer, BaseSerializationProxy& proxy)
{
	const SerializationConfigData& serializationData = GetEngineConfigData(outer).serializationConfigData;

	std::string className;
	std::string prefabPath;
	if (proxy.Get<std::string>(serializationData.prefabPathPropertyName, prefabPath))
	{
		json* j = GetJsonResource(prefabPath, outer);
		if (j == nullptr || !JSON::TryGetMember<std::string>(*j, serializationData.objectClassPropertyName, className))
		{
			__debugbreak();
			return nullptr;
		}
	}
	else if (!proxy.Get<std::string>(serializationData.objectClassPropertyName, className))
	{
		__debugbreak();
		return nullptr;
	}

	return CreateObjectOfClassDeferred<ObjectT>(className);
}
