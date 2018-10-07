#include "BaseService.h"
#include <cassert>

BaseService::BaseService(GameInstance* gameInstance)
	: BaseObject(gameInstance)
{
	assert(_owningInstance != nullptr);
}