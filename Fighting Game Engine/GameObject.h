#pragma once
#include <string>
#include <memory>
#include <map>
#include <functional>
#include "JSON.h"
#include "SceneObject.h"

class Transform;
class Mesh;
class Material;
class Renderer;
class Behaviour;

class GameObject : public SceneObject
{
	friend class GameScene;

private:
	unsigned int _id;

	std::map<std::string, std::unique_ptr<Behaviour>> _behaviours;

	GameInstance* _serviceManager;

public:
	bool enabled;

	int id() const;

	Behaviour* AddBehaviour(std::string name);

	void RunFunctionOnBehaviours(std::function<void(Behaviour*)> func);

	template<typename T>
	T* GetBehaviour(std::string name);

	GameObject();
	~GameObject() = default;
};

template<typename T>
inline T* GameObject::GetBehaviour(std::string name)
{
	auto iter = _behaviours.find(name);
	if(iter == _behaviours.end())
	{
		return nullptr;
	}
	
	return dynamic_cast<T*>(iter->second.get());
}
