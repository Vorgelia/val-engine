#include "ObjectPtr.h"
#include "BaseObject.h"

void ValEngine::ObjectDeleter::operator()(BaseObject* object) const
{
	object->OnDestroyed();
	object->InvalidateReferences();
	delete object;
}
