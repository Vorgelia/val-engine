#include "BaseObject.h"
#include "ObjectReferenceManager.h"
#include "GameInstance.h"

VE_OBJECT_DEFINITION(BaseObject)

json BaseObject::Serialize() const
{
	json outJson = IReflectable::Serialize();
	outJson.emplace(_owningInstance->configData().serializationConfigData.objectClassPropertyName, className());
	return outJson;
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
	ObjectReferenceManager::Get().RemoveObject(this);
}
