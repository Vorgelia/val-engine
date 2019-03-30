#pragma once
#include <string>
#include "JSON.h"
#include "SceneObject.h"

class GameObject;

class ObjectComponent : public SceneObject
{
	friend class ObjectInitializer;

protected:
	ObjectReference<GameObject> _owner;

public:
	bool enabled;

	GameObject* owner() const { return _owner.get(); }

	void RegisterReflectionFields() const override;

	ObjectComponent();
	virtual ~ObjectComponent() = default;
};