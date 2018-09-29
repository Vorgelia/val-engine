#pragma once
#include "BaseObject.h"
#include "Transform.h"

class GameObject :	public BaseObject
{
	friend class GameScene;

protected:
	Transform _localTransform;
	BaseObject* _transformParent;

public:
	const Transform& GetLocalTransform() const { return _localTransform; }
	Transform GetWorldTransform() const;

	void SetLocalTransform(Transform transform) { _localTransform = std::move(transform); }
	void SetWorldTransform(const Transform& transform);

	BaseObject* GetTransformParent() const { return _transformParent; }
	void SetTransformParent(const BaseObject* parent);

	virtual void OnPostInit() {}
	virtual void OnDestroy() {}

	GameObject() = default;
	~GameObject() = default;
};
