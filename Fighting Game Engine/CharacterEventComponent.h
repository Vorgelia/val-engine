#pragma once
#include "CharacterCollisionResult.h"
#include "BaseScriptVariable.h"
#include "BaseGameCharacterComponent.h"

struct CharacterCollisionResult;

class CharacterEventComponent : public BaseGameCharacterComponent
{
	friend class GameCharacter;

public:
	void HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, std::shared_ptr<BaseScriptVariable> hitReactionData);
	//return: Generated hit reaction data.
	std::shared_ptr<BaseScriptVariable> HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult);

	bool ResolveTrade(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived);
	void HandleTradeUnresolved(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived);
	void HandleTradeSuccess(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, std::shared_ptr<BaseScriptVariable> hitReactionFlags);

	CharacterEventComponent() = default;
	~CharacterEventComponent() = default;
};