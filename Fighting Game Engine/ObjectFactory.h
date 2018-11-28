#pragma once
#include "JSON.h"
#include "FilesystemManager.h"
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
	GameInstance* _gameInstance;

	static BaseObjectGeneratorMap* objectGenerators();

	const EngineConfigData& GetEngineConfigData() const;

public:
	template<typename ObjectT>
	static bool RegisterObjectGenerator(const std::string& name);
	
	template<typename ObjectT>
	ve::unique_object_ptr<ObjectT> CreateObject(BaseObject* outer, const json& j = json()); 
	template<typename ObjectT>
	ve::unique_object_ptr<ObjectT> CreateObjectDeferred();

	template<typename ObjectT = BaseObject>
	ve::unique_object_ptr<ObjectT> CreateObjectOfClass(const std::string& className, BaseObject* outer, const json& j = json());
	template<typename ObjectT = BaseObject>
	ve::unique_object_ptr<ObjectT> CreateObjectFromJson(BaseObject* outer, const json& j = json());

	static void InitializeObject(BaseObject* object, BaseObject* outer, const json& j = json());

	explicit ObjectFactory(GameInstance* gameInstance);
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

	InitializeObject(outer, object.get(), j);

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
	static_assert(std::is_base_of_v<BaseObject, ObjectT>);

	const auto* generators = objectGenerators();

	auto& generatorIter = generators->find(className);
	if(generatorIter == generators->end())
	{
		return nullptr;
	}

	ve::unique_object_ptr<ObjectT> object{ dynamic_cast<ObjectT*>(generatorIter->second()) };

	InitializeObject(outer, object.get(), j);

	return object;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObjectFromJson(BaseObject* outer, const json& j)
{
	const std::string& classPropertyName = GetEngineConfigData().serializationConfigData.objectClassPropertyName;

	std::string className{};
	if(!JSON::TryGetMember(j, classPropertyName, className))
	{
		return nullptr;
	}

	return CreateObjectOfClass<ObjectT>(className, outer, j);
}
