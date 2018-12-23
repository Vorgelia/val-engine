#pragma once
#include "BaseObject.h"
#include "Transform.h"
#include "ObjectReferenceManager.h"

class SceneObject :	public BaseObject
{
	friend class ObjectInitializer;

private:
	GameScene* _owningScene;

protected:
	std::string _name;

	Transform _localTransform;
	ObjectReference<SceneObject> _transformParent;
	
public:
	const std::string& name() const { return _name; }
	GameScene* owningScene() const { return _owningScene; }
	const Transform& localTransform() const { return _localTransform; }
	Transform& localTransform() { return _localTransform; }
	SceneObject* transformParent() const { return _transformParent.get(); }

	Transform GetWorldTransform() const;

	void SetLocalTransform(Transform transform) { _localTransform = std::move(transform); }
	void SetWorldTransform(const Transform& transform);

	void SetTransformParent(SceneObject* parent);

	virtual void RegisterReflectionFields() const override;

	SceneObject();
	~SceneObject() = default;
};
