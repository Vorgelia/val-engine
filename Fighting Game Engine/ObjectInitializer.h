#pragma once

class BaseObject;
class SceneObject;
class GameScene;
class GameObject;
class GameInstance;

class ObjectInitializer
{
protected:
	BaseObject* _outer;
	GameObject* _outerAsGameObject;
	GameInstance* _outerInstance;
	GameScene* _outerScene;

public:
	void InitializeObject(BaseObject* other) const;

	ObjectInitializer(BaseObject* outer);
	~ObjectInitializer() = default;
};