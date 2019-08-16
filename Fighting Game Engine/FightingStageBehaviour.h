#pragma once
#include "ValEngine.h"
#include "ObjectComponent.h"
#include "MathIncludes.hpp"
#include "GameCharacter.h"
#include "BaseSceneBehavior.h"

struct CharacterCollisionResult;
class FightingGameManager;

class FightingStageBehaviour : public BaseSceneBehavior
{
	VE_OBJECT_DECLARATION(FightingStageBehaviour, BaseSceneBehavior);

protected:
	FightingGameManager* _gameManager;

protected:
	ve::vec4 _stageBounds;
	//TODO: Make characters send scene events.
	//void HandleSceneEvent();

	void RegisterReflectionFields() const override;

	typedef std::map <GameCharacter*, std::vector<CharacterCollisionResult>, CharacterSortingPredicate> CharacterCollisionResultMap;
	virtual CharacterCollisionResultMap GenerateCharacterCollisionResults() const;

	virtual void HandleAttackHit(GameCharacter* attacker, GameCharacter* attackReceiver, const AttackCollisionHit& hit, bool wasTrade = false);

public:
	const ve::vec4& stageBounds() const { return _stageBounds; }

	void OnInit() override;
	void OnDestroyed() override;

	void GameUpdate();
	void LateGameUpdate();

	FightingStageBehaviour() = default;
	~FightingStageBehaviour() = default;
};