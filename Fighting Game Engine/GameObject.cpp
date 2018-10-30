#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"

ObjectReference<ObjectComponent> GameObject::AddComponentFromJson(const json& j)
{
	_components.push_back(_owningInstance->objectFactory().CreateObjectFromJson<ObjectComponent>(this, j));
	ObjectComponent* result = (_components.back().get());

	return ObjectReference<ObjectComponent>(result);
}

GameObject::GameObject()
{
	
}