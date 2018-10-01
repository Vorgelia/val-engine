#pragma once
#include "IReflectable.h"

class GameScene;
class GameInstance;

class BaseObject : public IReflectable
{
	friend class GameInstance;

protected:
	BaseObject* _owner;

public:
	virtual BaseObject* GetOwner() const;
	virtual GameScene* GetOwningScene() const;
	virtual GameInstance* GetOwningInstance() const;

	virtual void OnPostInit() {}
	virtual void OnDestroy() {}

	BaseObject();
	virtual ~BaseObject() = default;
};