#include "GameCharacterComponent.h"


GameCharacterComponent::GameCharacterComponent(GameCharacter* owner, ServiceManager* serviceManager)
	: _owner(owner)
	, _serviceManager(serviceManager)
{

}
