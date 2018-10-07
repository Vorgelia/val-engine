#pragma once
#include <string>
#include "JSON.h"
#include "SceneObject.h"

class GameObject;

class ObjectComponent : public SceneObject
{
	friend class ObjectInitializer;

protected:
	GameObject* _owner;

public:
	bool enabled;

	GameObject* owner() const { return _owner; }

	void RegisterReflectionFields() const override;

	ObjectComponent(GameInstance* owningInstance);
	virtual ~ObjectComponent() = default;
};