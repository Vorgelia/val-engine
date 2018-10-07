#include "ObjectReferenceManager.h"

ObjectReferenceManager& ObjectReferenceManager::Get()
{
	if(_instance == nullptr)
	{
		_instance = std::make_unique<ObjectReferenceManager>();
	}
	return *_instance;
}

void ObjectReferenceManager::AddObject(const BaseObject* object)
{
	_objectLookup.emplace(object);
}

void ObjectReferenceManager::RemoveObject(const BaseObject* object)
{
	_objectLookup.erase(object);
}

bool ObjectReferenceManager::IsObjectValid(const BaseObject* object)
{
	return (object != nullptr) && (_objectLookup.find(object) != _objectLookup.end());
}