#include "CharacterEventComponent.h"
#include "CharacterCollisionResult.h"


void CharacterEventComponent::Init()
{
}

void CharacterEventComponent::Update()
{
}

void CharacterEventComponent::HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult, const std::vector<std::string>& hitReactionFlags)
{
}

std::vector<std::string> CharacterEventComponent::HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult)
{
	return std::vector<std::string>();
}

bool CharacterEventComponent::ResolveTrade(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived)
{
	return false;
}

void CharacterEventComponent::HandleTradeUnresolved(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived)
{
}

void CharacterEventComponent::HandleTradeSuccess(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const std::vector<std::string>& hitReactionFlags)
{
}

CharacterEventComponent::CharacterEventComponent(GameCharacter* owner, ServiceManager* serviceManager)
	: GameCharacterComponent(owner, serviceManager)
{

}
