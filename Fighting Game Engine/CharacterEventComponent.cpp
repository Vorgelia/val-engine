#include "CharacterEventComponent.h"
#include "CharacterCollisionResult.h"


void CharacterEventComponent::Init()
{
}

void CharacterEventComponent::Update()
{
}


CharacterEventComponent::CharacterEventComponent(GameCharacter* owner, ServiceManager* serviceManager)
	: GameCharacterComponent(owner, serviceManager)
{
}
