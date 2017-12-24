#include "BaseService.h"
#include "DebugLog.h"

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
	_serviceManager = serviceManager;
	_allowServiceUpdate = false;
}

BaseService::~BaseService()
{
	
}
