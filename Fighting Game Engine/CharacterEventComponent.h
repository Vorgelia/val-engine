#pragma once
#include "GameCharacterComponent.h"
#include "CharacterCollisionResult.h"

struct CharacterCollisionResult;

class CharacterEventComponent : public GameCharacterComponent
{
	friend class GameCharacter;

protected:
	void Init() override;
	void Update() override;

public:
	void HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, std::shared_ptr<BaseScriptVariable> hitReactionData);
	//return: Generated hit reaction data.
	std::shared_ptr<BaseScriptVariable> HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult);

	bool ResolveTrade(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived);
	void HandleTradeUnresolved(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived);
	void HandleTradeSuccess(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, std::shared_ptr<BaseScriptVariable> hitReactionFlags);

	CharacterEventComponent(GameCharacter* owner, GameInstance* serviceManager);
	~CharacterEventComponent() = default;
};