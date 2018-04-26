#pragma once
#include "GameCharacterComponent.h"

class CharacterEventComponent : public GameCharacterComponent
{
	friend class GameCharacter;

protected:
	void Init() override;
	void Update() override;

public:
	void HandleAttackHit();
	void HandleAttackReceived();
	void HandleAttackTraded();

	CharacterEventComponent(GameCharacter* owner, ServiceManager* serviceManager);
	~CharacterEventComponent() = default;
};

