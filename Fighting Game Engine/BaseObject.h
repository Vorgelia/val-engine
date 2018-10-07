#pragma once
#include "IReflectable.h"
#include "ObjectFactory.h"

class GameScene;
class GameInstance;

class BaseObject : public IReflectable
{
	friend class ObjectInitializer;

protected:
	GameInstance* _owningInstance;
	BaseObject* _outer;

public:
	GameInstance* owningInstance() const { return _owningInstance; }

	virtual void OnInit() {}
	virtual void OnDestroyed() {}

	BaseObject();
	virtual ~BaseObject();
};