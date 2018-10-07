#pragma once
#include "JSON.h"
#include "ObjectInitializer.h"

class GameInstance;
class GameScene;
class BaseObject;
class SceneObject;
class GameObject;
class ObjectComponent;

#define VE_REGISTER_OBJECT_GENERATOR(GenType) bool _registered##GenType = ObjectFactory::RegisterObjectGenerator<GenType>(#GenType)

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

	void InitializeObject(BaseObject* object, BaseObject* outer, const json& j);

public:
	template<typename ObjectT>
	static bool RegisterObjectGenerator(const std::string& name);
	
	template<typename ObjectT>
	ve::unique_object_ptr<ObjectT> CreateObject(BaseObject* outer, const json& j = json());
	template<typename ObjectT = BaseObject>
	ve::unique_object_ptr<ObjectT> CreateObject(const std::string& className, BaseObject* outer, const json& j = json());
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
	static_assert(std::is_base_of_v<BaseObject, ObjectT>);

	ve::unique_object_ptr<ObjectT> object{ new ObjectT() };

	InitializeObject(outer, object.get(), j);

	return object;
}

template <typename ObjectT>
ve::unique_object_ptr<ObjectT> ObjectFactory::CreateObject(const std::string& className, BaseObject* outer, const json& j)
{
	static_assert(std::is_base_of_v<BaseObject, ObjectT>);

	const auto* generators = objectGenerators();

	auto& generatorIter = generators->find(className);
	if(generatorIter == generators->end())
	{
		return nullptr;
	}

	ve::unique_object_ptr<ObjectT> object{ generatorIter->second() };

	InitializeObject(outer, object.get(), j);

	return object;
}
