#pragma once
#include <memory>
#include <string>
#include <functional>
#include <unordered_map>
#include "JSON.h"

#define VE_BEHAVIOUR_REGISTER_TYPE(name)\
	bool registered##name = BehaviourFactory::RegisterType<name>(std::string(#name))

class Object;
class Behaviour;
class GameInstance;

typedef std::function<std::unique_ptr<Behaviour>(Object*, GameInstance*, const json&)> BehaviourGenerator;
typedef std::unordered_map<std::string, BehaviourGenerator> BehaviourGeneratorMap;

class BehaviourFactory
{
	static BehaviourGeneratorMap* _objectGenerators();
public:
	template<typename T>
	static bool RegisterType(std::string name);
	static std::unique_ptr<Behaviour> Create(std::string name, Object* owner, GameInstance* serviceManager, const json& j);
};

template<typename T>
inline bool BehaviourFactory::RegisterType(std::string name)
{
	auto& iter = _objectGenerators()->find(name);
	if(iter == _objectGenerators()->end())
	{
		_objectGenerators()->emplace(name, 
			[](Object* owner, GameInstance* serviceManager, const json& j)->std::unique_ptr<T> { return std::make_unique<T>(owner, serviceManager, j); });
		return true;
	}

	return false;
}
