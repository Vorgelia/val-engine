#include "BaseObject.h"

BaseObject* BaseObject::GetOwner() const
{
	return _owner;
}

GameScene* BaseObject::GetOwningScene() const
{
	if(_owner == nullptr)
	{
		return nullptr;
	}

	return _owner->GetOwningScene();
}

GameInstance* BaseObject::GetOwningInstance() const
{
	if(_owner == nullptr)
	{
		return nullptr;
	}

	return _owner->GetOwningInstance();
}

BaseObject::BaseObject()
	: _owner(nullptr)
{
	
}
