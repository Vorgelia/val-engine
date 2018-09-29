#include "ServiceUpdateDispatcher.h"

void ServiceUpdateDispatcher::BindFunction(ServiceUpdateFunction function)
{
	_boundFunctions.push_back(std::move(function));
}

void ServiceUpdateDispatcher::DispatchUpdates()
{

}

ServiceUpdateDispatcher::ServiceUpdateDispatcher(GameInstance* gameInstance)
	: _gameInstance(gameInstance)
{
	
}
