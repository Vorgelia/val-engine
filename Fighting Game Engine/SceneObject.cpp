#include "SceneObject.h"

Transform SceneObject::GetWorldTransform() const
{
	if(_transformParent.get() == nullptr)
	{
		return _localTransform;
	}

	return _transformParent.get()->GetWorldTransform() * _localTransform;
}

void SceneObject::RegisterReflectionFields() const
{
	BaseObject::RegisterReflectionFields();
	AddReflectionField(VE_PRIVATE_REFLECTION_ARG(name));
}

void SceneObject::SetWorldTransform(const Transform& transform)
{
	if(_transformParent.get() == nullptr)
	{
		_localTransform = transform;
		return;
	}

	_localTransform = _transformParent->GetWorldTransform().GetInverse() * transform;
}

void SceneObject::SetTransformParent(SceneObject* parent)
{
	const Transform currentWorldTransform = GetWorldTransform();
	_transformParent = parent;
	SetWorldTransform(currentWorldTransform);
}

SceneObject::SceneObject()
	: _owningScene(nullptr)
	, _transformParent(nullptr)
{
	
}
