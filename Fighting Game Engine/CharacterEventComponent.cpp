#include "CharacterEventComponent.h"
#include "CharacterCollisionResult.h"
#include "GameCharacter.h"
#include "CharacterStateComponent.h"
#include "CharacterState.h"
#include "Script.h"
#include "ScriptMap.h"
#include "GameInstance.h"
#include "FightingGameManager.h"


void CharacterEventComponent::Init()
{
}

void CharacterEventComponent::Update()
{
}

void CharacterEventComponent::HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, std::shared_ptr<BaseScriptVariable> hitReactionFlags)
{
	_owner->stateComponent()->_usedHitboxSequenceIDs.emplace(attackHit.hitbox.sequenceID());
	_owner->stateComponent()->currentState()->script()->CallBoundFunction(
		"HandleAttackHit", 
		ScriptArgumentCollection
		{
			ScriptVariableUtils::FromReflectable(attackHit),
			hitReactionFlags
		}
	);
}

std::shared_ptr<BaseScriptVariable> CharacterEventComponent::HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& attackReceived)
{
	return _owner->stateComponent()->currentState()->script()->CallBoundFunction(
		"HandleAttackReceived"
		, ScriptArgumentCollection
		{
			ScriptVariableUtils::FromReflectable(attackReceived)
		}
	);
}

bool CharacterEventComponent::ResolveTrade(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived)
{
	const std::shared_ptr<BaseScriptVariable> result = _owner->stateComponent()->currentState()->script()->CallBoundFunction(
		"ResolveTrade"
		, ScriptArgumentCollection
		{
			ScriptVariableUtils::FromReflectable(attackHit), 
			ScriptVariableUtils::FromReflectable(attackReceived)
		}
	);

	const std::shared_ptr<ScriptBool> resultAsBool = ScriptVariableUtils::Cast<ScriptBool>(result);
	if(resultAsBool == nullptr)
	{
		return false;
	}

	return resultAsBool->value();
}

void CharacterEventComponent::HandleTradeUnresolved(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived)
{
	_owner->stateComponent()->_usedHitboxSequenceIDs.emplace(attackHit.hitbox.sequenceID());
	_owner->stateComponent()->currentState()->script()->CallBoundFunction(
		"HandleTradeUnresolved"
		, ScriptArgumentCollection
			{
				ScriptVariableUtils::FromReflectable(attackHit),
				ScriptVariableUtils::FromReflectable(attackReceived)
			}
	);
}

void CharacterEventComponent::HandleTradeSuccess(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, std::shared_ptr<BaseScriptVariable> hitReactionFlags)
{
	_owner->stateComponent()->_usedHitboxSequenceIDs.emplace(attackHit.hitbox.sequenceID());
	_owner->stateComponent()->currentState()->script()->CallBoundFunction(
		"HandleTradeSuccess", 
		ScriptArgumentCollection
			{
				ScriptVariableUtils::FromReflectable(attackHit),
				hitReactionFlags
			}
	);
}

CharacterEventComponent::CharacterEventComponent(GameCharacter* owner, GameInstance* serviceManager)
	: GameCharacterComponent(owner, serviceManager)
{

}
