#pragma once
#include <string>
#include <memory>
#include <map>
#include <GLM\glm.hpp>
#include <functional>
#include "JSON.h"
#include "BehaviourFactory.h"
class Transform;
class Mesh;
class Material;
class Renderer;
class Behaviour;

class Object
{
	std::string _name;
	unsigned int _id;

	Transform* _transform;
	Renderer* _renderer;

	std::map<std::string, std::unique_ptr<Behaviour>> _behaviours;

public:
	bool enabled;

	std::string name() const;
	int id() const;

	Transform* transform() const;
	Renderer* renderer() const;

	template <typename ... Types>
	Behaviour* AddBehaviour(std::string name, Types ... args);

	void RunFunctionOnBehaviours(std::function<void(Behaviour*)> func);

	template<typename T>
	T* GetBehaviour(std::string name);

	Object(const std::string& name, int id = 0);
	Object(const nlohmann::json& j);
	~Object() = default;
};

template<typename ... Types>
Behaviour* Object::AddBehaviour(std::string behaviourName, Types ... args)
{
	Behaviour* behaviour = _behaviours.emplace(
		std::make_pair(
			behaviourName,
			BehaviourFactory::Create(behaviourName, this, args...))
	).first->second.get();

	if(behaviour->usingInit())
	{
		behaviour->Init();
	}

	return behaviour;
}

template<typename T>
inline T* Object::GetBehaviour(std::string name)
{
	auto& iter = _behaviours.find(name);
	if(iter == _behaviours.end())
	{
		return nullptr;
	}

	return dynamic_cast<T*>(iter->second.get());
}
