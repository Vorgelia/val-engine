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
	void HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult, const std::vector<std::string>& hitReactionFlags);
	//return: Generated hit flags.
	std::vector<std::string> HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult);

	bool ResolveTrade(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived);
	void HandleTradeUnresolved(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const AttackCollisionHit& attackReceived);
	void HandleTradeSuccess(GameCharacter* otherCharacter, const AttackCollisionHit& attackHit, const std::vector<std::string>& hitReactionFlags);

	CharacterEventComponent(GameCharacter* owner, ServiceManager* serviceManager);
	~CharacterEventComponent() = default;
};

