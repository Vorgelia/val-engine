#pragma once
#include "Behaviour.h"
#include "MathIncludes.hpp"

struct CharacterCollisionResult;
class GameCharacter;
class FightingGameManager;

class FightingStageBehaviour :
	public Behaviour
{
protected:
	FightingGameManager* _gameManager;

protected:
	glm::lvec4 _stageBounds;
	//TODO: Make characters send scene events.
	//void HandleSceneEvent();

	virtual std::unordered_map<GameCharacter*, std::vector<CharacterCollisionResult>> GenerateCharacterCollisionResults() const;

public:
	VE_BEHAVIOUR_NAME(FightingStageBehaviour);

	const glm::lvec4& stageBounds() const;

	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);
	VE_BEHAVIOUR_REGISTER_FUNCTION(LateGameUpdate);

	FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j);
	~FightingStageBehaviour() = default;
};