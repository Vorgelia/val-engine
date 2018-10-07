#include "ObjectComponent.h"
void ObjectComponent::RegisterReflectionFields() const
{
	SceneObject::RegisterReflectionFields();
	AddReflectionField(VE_REFLECTION_ARG(enabled));
}

ObjectComponent::ObjectComponent(GameInstance* owningInstance)
	: SceneObject(owningInstance)
	, _owner(nullptr)
	, enabled(true)
{
	
}
