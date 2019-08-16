#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"

VE_OBJECT_DEFINITION(GameObject);

void GameObject::RegisterReflectionFields() const
{
	SceneObject::RegisterReflectionFields();
	VE_PRIVATE_REFLECTION_VAR(ObjectArray, components, this);
}
