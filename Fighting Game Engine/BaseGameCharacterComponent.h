#pragma once
#include "ObjectComponent.h"

class GameCharacter;
class FightingStageBehaviour;
class FightingGameManager;

class BaseGameCharacterComponent : public ObjectComponent
{
protected:
	ObjectReference<GameCharacter> _gameCharacterOwner;
	ObjectReference<FightingStageBehaviour>	_fightingSceneBehavior;
	ObjectReference<FightingGameManager> _fightingGameManager;

public:
	void OnInit() override;
};

