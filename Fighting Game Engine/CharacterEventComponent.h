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
	std::vector<std::string> EarlyHandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult, const std::vector<std::string>& hitReactionFlags);
	std::vector<std::string> HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult, const std::vector<std::string>& hitReactionFlags);
	std::vector<std::string> HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult);

	CharacterEventComponent(GameCharacter* owner, ServiceManager* serviceManager);
	~CharacterEventComponent() = default;
};

