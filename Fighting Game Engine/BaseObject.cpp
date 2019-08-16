#include "BaseObject.h"
#include "ObjectReferenceManager.h"
#include "GameInstance.h"

VE_OBJECT_DEFINITION(BaseObject)

void BaseObject::OnSerialized(BaseSerializationProxy& proxy) const
{
	proxy.Set<std::string>(_owningInstance->configData().serializationConfigData.objectClassPropertyName, className());
}

void BaseObject::RegisterReference(const ObjectReference<BaseObject>& reference)
{
	_references.emplace(&reference);
}

void BaseObject::UnregisterReference(const ObjectReference<BaseObject>& reference)
{
	_references.erase(&reference);
}

void BaseObject::InvalidateReferences()
{
	if (GetClass() != this)
	{
		for (auto& iter : _references)
		{
			iter->Invalidate();
		}
		if (ObjectReferenceManager::TryGet())
		{
			ObjectReferenceManager::Get().RemoveObject(this);
		}
	}
}

BaseObject::BaseObject()
	: _owningInstance(nullptr)
	, _outer(nullptr)
	, enabled(true)
{
	ObjectReferenceManager::Get().AddObject(this);
}

BaseObject::~BaseObject()
{

}
