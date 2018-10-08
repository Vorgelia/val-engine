#include "BaseService.h"
#include <cassert>

BaseService::BaseService()
{
	assert(_owningInstance != nullptr);
}