#include "BaseService.h"
#include <cassert>

bool BaseService::allowServiceUpdate() const
{
	return _allowServiceUpdate;
}

int BaseService::serviceUpdateSortingOrder() const
{
	return _serviceUpdateSortingOrder;
}

BaseService::BaseService(ServiceManager* serviceManager, int sortingOrder) :
	_serviceUpdateSortingOrder(sortingOrder)
{
	assert(serviceManager != nullptr);
	_serviceManager = serviceManager;
	_allowServiceUpdate = false;
}

BaseService::~BaseService()
= default;
