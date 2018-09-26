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

GameInstance* BaseObject::GetGameInstance() const
{
	if(_owner == nullptr)
	{
		return nullptr;
	}

	return _owner->GetGameInstance();
}

BaseObject::BaseObject(BaseObject* owner)
	: _owner(owner)
{
	
}
