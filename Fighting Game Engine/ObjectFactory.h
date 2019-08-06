#pragma once
#include "JSON.h"
#include "ObjectInitializer.h"
#include "EngineConfigData.h"

class GameInstance;
class GameScene;
class BaseObject;
class SceneObject;
class GameObject;
class ObjectComponent;

namespace ValEngine
{
	struct ObjectDeleter
	{
		void operator()(BaseObject* object) const;
	};

	template <typename ObjectT>
	using unique_object_ptr = std::unique_ptr<ObjectT, ObjectDeleter>;
}

class ObjectFactory
{
	typedef std::function<BaseObject*()> BaseObjectGenerator;
	typedef std::unordered_map<std::string, BaseObjectGenerator> BaseObjectGeneratorMap;

protected:
	static BaseObjectGeneratorMap* objectGenerators();

	static const EngineConfigData& GetEngineConfigData(BaseObject* contextObject);
	static void CreateComponents(GameObject* object, const json& j);

public:
	template<typename ObjectT>
	static bool RegisterObjectGenerator(const std::string& name);
	
	template<typename ObjectT>
	static ve::unique_object_ptr<ObjectT> CreateObject(BaseObject* outer, const json& j = json()); 
	template<typename ObjectT>
	static ve::unique_object_ptr<ObjectT> CreateObjectDeferred();

	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectOfClass(const std::string & className, BaseObject* outer, const json& j = json());
	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectOfClassDeferred(const std::string& className);
	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectFromJson(BaseObject* outer, const json& j = json());
	template<typename ObjectT = BaseObject>
	static ve::unique_object_ptr<ObjectT> CreateObjectFromJsonDeferred(BaseObject * outer, const json& j);

	static void InitializeObject(BaseObject* object, BaseObject* outer, const json& j = json());

	ObjectFactory() = delete;
};

namespace ve = ValEngine;

template <typename ObjectT>
bool ObjectFactory::RegisterObjectGenerator(const std::string& name)
{
	objectGenerators()->emplace(name, []()->BaseObject* { return new ObjectT(); });
	return true;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObject(BaseObject* outer, const json& j)
{
	ve::unique_object_ptr<ObjectT> object = CreateObjectDeferred<ObjectT>();

	InitializeObject(object.get(), outer, j);

	return object;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectDeferred()
{
	static_assert(std::is_base_of_v<BaseObject, ObjectT>);

	return ve::unique_object_ptr<ObjectT> { new ObjectT() };
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectOfClass(const std::string& className, BaseObject* outer, const json& j)
{
	ve::unique_object_ptr<ObjectT> object = CreateObjectOfClassDeferred<ObjectT>(className);

	InitializeObject(object.get(), outer, j);

	return object;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectOfClassDeferred(const std::string& className)
{
	static_assert(std::is_base_of_v<BaseObject, ObjectT>);

	const auto* generators = objectGenerators();

	auto& generatorIter = generators->find(className);
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
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectFromJson(BaseObject* outer, const json& j)
{
	ve::unique_object_ptr<ObjectT> object = CreateObjectFromJsonDeferred<ObjectT>(outer, j);

	InitializeObject(object.get(), outer, j); 
	
	return object;
}

template<typename ObjectT>
inline ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectFromJsonDeferred(BaseObject* outer, const json& j)
{
	const std::string& classPropertyName = GetEngineConfigData(outer).serializationConfigData.objectClassPropertyName;

	std::string className{};
	if (!JSON::TryGetMember(j, classPropertyName, className))
	{
		__debugbreak();
		return nullptr;
	}

	return CreateObjectOfClassDeferred<ObjectT>(className);
}
