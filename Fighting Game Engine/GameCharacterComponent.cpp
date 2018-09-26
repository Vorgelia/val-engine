#include "GameCharacterComponent.h"


GameCharacterComponent::GameCharacterComponent(GameCharacter* owner, GameInstance* serviceManager)
	: _owner(owner)
	, _serviceManager(serviceManager)
{

}
