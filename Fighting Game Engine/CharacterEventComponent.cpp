#include "CharacterEventComponent.h"
#include "CharacterCollisionResult.h"
#include "GameCharacter.h"
#include "CharacterStateComponent.h"
#include "CharacterState.h"
#include "Script.h"
#include "ScriptMap.h"


void CharacterEventComponent::Init()
{
}

void CharacterEventComponent::Update()
{
}

void CharacterEventComponent::HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const std::vector<std::string>& hitReactionFlags)
{
	_owner->stateComponent()->_usedHitboxSequenceIDs.emplace(attackHit.hitbox.sequenceID());
	_owner->stateComponent()->currentState()->script()->CallBoundFunction("HandleAttackHit"
		, ScriptArgumentCollection{ 
			std::make_shared<ScriptMap>(attackHit.hitbox.data()),
			std::make_shared<ScriptMap>(attackHit.hurtbox.data())
		});
}

std::vector<std::string> CharacterEventComponent::HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult)
{
	_owner->stateComponent()->currentState()->script()->CallBoundFunction("HandleAttackReceived"
		, ScriptArgumentCollection{
			std::make_shared<ScriptMap>(collisionResult.hitbox.data()),
			std::make_shared<ScriptMap>(collisionResult.hurtbox.data())
		});
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
