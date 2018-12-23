#pragma once
#include "ObjectComponent.h"
#include "MathIncludes.hpp"
#include <vector>
#include "CharacterCollisionResult.h"
#include "BaseGameCharacterComponent.h"

class GameInstance;
class FightingGameManager;

class CharacterPhysicsComponent : public BaseGameCharacterComponent
{
	friend class GameCharacter;

	struct CharacterPhysicsForce
	{
		ve::int_t duration;
		ve::vec2 force;
	};

	struct CharacterPhysicsParamOverride
	{
		ve::int_t duration;
		ve::dec_t value;
	};

private:
	ve::vec2 _accumulatedOffset;
	ve::vec2 _velocity;

	std::vector<CharacterPhysicsForce> _forces;

	CharacterPhysicsParamOverride _gravityOverride;
	CharacterPhysicsParamOverride _groundFrictionOverride;
	CharacterPhysicsParamOverride _airFrictionOverride;

	bool _grounded;

	void Update();
	void LateUpdate();

	void ApplyAccumulatedOffset();
	void UpdateForces();
	void ApplyFriction();

public:
	void AddOffset(ve::vec2 dir);

	ve::vec2 GetVelocity() const;
	void AddVelocity(ve::vec2 dir);
	void SetVelocity(ve::vec2 dir);

	void AddForce(ve::int_t duration, ve::vec2 dir);

	void OverrideGravity(ve::int_t duration, ve::dec_t value) { _gravityOverride = { duration, value }; }
	void OverrideGroundFriction(ve::int_t duration, ve::dec_t value) { _groundFrictionOverride = { duration, value }; }
	void OverrideAirFriction(ve::int_t duration, ve::dec_t value) { _airFrictionOverride = { duration, value }; }

	void HandleCharacterCollision(const GameCharacter* other, const CollisionHit collision);

	CharacterPhysicsComponent() = default;
	~CharacterPhysicsComponent() = default;
};