#pragma once
#include "ValEngine.h"
#include "Behaviour.h"
#include "MathIncludes.hpp"
#include "GameCharacter.h"

struct CharacterCollisionResult;
class FightingGameManager;

class FightingStageBehaviour :
	public Behaviour
{
protected:
	FightingGameManager* _gameManager;

protected:
	ve::vec4 _stageBounds;
	//TODO: Make characters send scene events.
	//void HandleSceneEvent();

	typedef std::map <GameCharacter*, std::vector<CharacterCollisionResult>, CharacterSortingPredicate> CharacterCollisionResultMap;
	virtual CharacterCollisionResultMap GenerateCharacterCollisionResults() const;

public:
	VE_BEHAVIOUR_NAME(FightingStageBehaviour);

	const ve::vec4& stageBounds() const;

	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);
	VE_BEHAVIOUR_REGISTER_FUNCTION(LateGameUpdate);

	FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j);
	~FightingStageBehaviour() = default;
};