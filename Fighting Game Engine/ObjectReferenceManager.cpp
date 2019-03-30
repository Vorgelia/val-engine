#include "ObjectReferenceManager.h"
#include "BaseObject.h"

std::unique_ptr<ObjectReferenceManager> ObjectReferenceManager::_instance = nullptr;

ObjectReferenceManager& ObjectReferenceManager::Get()
{
	if(_instance == nullptr)
	{
		_instance = std::unique_ptr<ObjectReferenceManager>(new ObjectReferenceManager());
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

void ObjectReferenceManager::RegisterReferenceToObject(BaseObject* object, const ObjectReference<BaseObject>& reference)
{
	if(IsObjectValid(object))
	{
		object->RegisterReference(reference);
	}
}

void ObjectReferenceManager::UnregisterReferenceToObject(BaseObject* object, const ObjectReference<BaseObject>& reference)
{
	if(IsObjectValid(object))
	{
		object->UnregisterReference(reference);
	}
}

bool ObjectReferenceManager::IsObjectValid(const BaseObject* object)
{
	return (object != nullptr) && (_objectLookup.find(object) != _objectLookup.end());
}

ObjectReferenceManager::~ObjectReferenceManager()
{
	_instance = nullptr;
}
