#pragma once

class GameScene;
class GameInstance;

class BaseObject
{
	friend class GameInstance;

protected:
	BaseObject* _owner;

public:
	virtual BaseObject* GetOwner() const;
	virtual GameScene* GetOwningScene() const;
	virtual GameInstance* GetOwningInstance() const;

	BaseObject();
	virtual ~BaseObject() = default;
};