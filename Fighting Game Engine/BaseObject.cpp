#include "BaseObject.h"
#include "ObjectReferenceManager.h"

BaseObject::BaseObject()
	: _owningInstance(nullptr)
{
	ObjectReferenceManager::Get().AddObject(this);
}

BaseObject::~BaseObject()
{
	ObjectReferenceManager::Get().RemoveObject(this);
}
