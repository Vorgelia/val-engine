#pragma once
#include <string>
#include <memory>
#include <map>
#include <GLM\glm.hpp>
#include <functional>
#include "JSON.h"

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

	template <typename T, typename ... Types>
	Behaviour* AddBehaviour(std::string name, Types ... args);

	void RunFunctionOnBehaviours(std::function<void(Behaviour*)> func);

	Object(const std::string& name, int id = 0);
	Object(const nlohmann::json& j);
	~Object() = default;
};

template<typename T, typename ... Types>
Behaviour* Object::AddBehaviour(std::string behaviourName, Types ... args)
{
	return _behaviours.emplace(
		std::pair<std::string, std::unique_ptr<T>>(
			behaviourName, 
			std::make_unique<T>(this, args...))
	).first->second.get();
}