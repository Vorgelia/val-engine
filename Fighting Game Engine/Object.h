#pragma once
#include <string>
#include <memory>
#include <map>
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
	friend class GameScene;

private:
	std::string _name;
	unsigned int _id;

	Transform* _transform;
	Renderer* _renderer;

	std::map<std::string, std::unique_ptr<Behaviour>> _behaviours;

	GameInstance* _serviceManager;

public:
	bool enabled;
	int updatePriority;

	std::string name() const;
	int id() const;

	Transform* transform() const;
	Renderer* renderer() const;

	template <typename ... Types>
	Behaviour* AddBehaviour(std::string name, Types ... args);

	void RunFunctionOnBehaviours(std::function<void(Behaviour*)> func);

	template<typename T>
	T* GetBehaviour(std::string name);

	Object(const std::string& name, GameInstance* serviceManager, int id = 0);
	Object(const json& j, GameInstance* serviceManager, int id = 0);
	~Object() = default;
};

template<typename ... Types>
Behaviour* Object::AddBehaviour(std::string name, Types ... args)
{
	Behaviour* behaviour = _behaviours.emplace(
		std::make_pair(
			name,
			BehaviourFactory::Create(name, this, _serviceManager, args...))
	).first->second.get();

	if(behaviour->usingInit())
	{
		behaviour->Init();
	}

	if(name == "Transform")
	{
		_transform = static_cast<Transform*>(behaviour);
	}
	else if(name == "Renderer")
	{
		_renderer = static_cast<Renderer*>(behaviour);
	}

	return behaviour;
}

template<typename T>
inline T* Object::GetBehaviour(std::string name)
{
	auto iter = _behaviours.find(name);
	if(iter == _behaviours.end())
	{
		return nullptr;
	}

	return dynamic_cast<T*>(iter->second.get());
}
