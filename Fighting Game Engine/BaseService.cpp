#include "BaseService.h"
#include <cassert>

VE_OBJECT_DEFINITION(BaseService)

BaseService::BaseService()
{
	assert(_owningInstance != nullptr);
}