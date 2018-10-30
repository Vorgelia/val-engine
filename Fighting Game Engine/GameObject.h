#pragma once
#include <string>
#include <memory>
#include <map>
#include <functional>
#include "JSON.h"
#include "SceneObject.h"
#include "GameInstance.h"

class Transform;
class Mesh;
class Material;
class Renderer;
class ObjectComponent;

class GameObject : public SceneObject
{
	friend class GameScene;

protected:
	std::vector<ve::unique_object_ptr<ObjectComponent>> _components;

public:
	template<typename ComponentT>
	ObjectReference<ComponentT> AddComponentOfType(const json& j = json());
	ObjectReference<ObjectComponent> AddComponentFromJson(const json& j = json());

	template<typename ComponentT>
	ObjectReference<ComponentT> GetComponentOfType() const;
	template<typename ComponentT>
	std::vector<ObjectReference<ComponentT>> GetComponentsOfType() const;

	GameObject();
	~GameObject() = default;
};

template <typename ComponentT>
ObjectReference<ComponentT> GameObject::AddComponentOfType(const json& j)
{
	_components.push_back(_owningInstance->objectFactory().CreateObject<ComponentT>(this, j));
	ComponentT* result = reinterpret_cast<ComponentT*>(_components.back().get());

	return ObjectReference<ComponentT>(result);
}

template <typename ComponentT>
ObjectReference<ComponentT> GameObject::GetComponentOfType() const
{
	for(auto& iter : _components)
	{
		ComponentT* component = dynamic_cast<ComponentT*>(iter);
		if(component != nullptr)
		{
			return ObjectReference<ComponentT>(component);
		}
	}

	return nullptr;
}

template <typename ComponentT>
std::vector<ObjectReference<ComponentT>> GameObject::GetComponentsOfType() const
{
	std::vector<ObjectReference<ComponentT>> outComponents;

	for(auto& iter : _components)
	{
		ComponentT* component = dynamic_cast<ComponentT*>(iter);
		if(component != nullptr)
		{
			outComponents.emplace_back(component);
		}
	}

	return outComponents;
}
