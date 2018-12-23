#include "ObjectComponent.h"
#include "GameObject.h"

void ObjectComponent::RegisterReflectionFields() const
{
	SceneObject::RegisterReflectionFields();
	AddReflectionField(VE_REFLECTION_ARG(enabled));
}

ObjectComponent::ObjectComponent()
	: _owner(nullptr)
	, enabled(true)
{
	
}
