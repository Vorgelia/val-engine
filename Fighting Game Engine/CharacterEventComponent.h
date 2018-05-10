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
	void HandleAttackHit(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult);
	void HandleAttackReceived(GameCharacter* otherCharacter, const AttackCollisionHit& collisionResult);

	CharacterEventComponent(GameCharacter* owner, ServiceManager* serviceManager);
	~CharacterEventComponent() = default;
};

