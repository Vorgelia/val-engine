#include "ValEngine.h"
#include "BaseObject.h"
#include "ObjectReferenceManager.h"

bool ValEngine::IsValid(const BaseObject* object)
{
	return ObjectReferenceManager::Get().IsObjectValid(object);
}
