#pragma once

class GameScene;
class GameInstance;

class BaseObject
{
protected:
	BaseObject* _owner;

public:
	BaseObject* GetOwner() const;
	GameScene* GetOwningScene() const;
	GameInstance* GetGameInstance() const;

	void OnPostInit() {}

	BaseObject(BaseObject* owner);
	virtual ~BaseObject() = default;
};