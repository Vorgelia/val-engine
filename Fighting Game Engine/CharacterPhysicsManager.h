#pragma once
#include "MathIncludes.hpp"
#include <vector>

class ServiceManager;
class FightingGameManager;


class CharacterPhysicsManager
{
	friend class GameCharacter;

	struct CharacterPhysicsForce
	{
		std::int64_t duration;
		glm::lvec2 force;
	};

	struct CharacterPhysicsParamOverride
	{
		std::int64_t duration;
		std::int64_t value;
	};

private:
	FightingGameManager* _fightingGameManager;
	GameCharacter* _owner;

private:
	glm::lvec2 _accumulatedOffset;
	glm::lvec2 _velocity;

	std::vector<CharacterPhysicsForce> _forces;

	CharacterPhysicsParamOverride _gravityOverride;
	CharacterPhysicsParamOverride _groundFrictionOverride;
	CharacterPhysicsParamOverride _airFrictionOverride;

	bool _grounded;

	void Update();

public:
	void AddOffset(glm::lvec2 dir);

	glm::lvec2 GetVelocity() const;
	void AddVelocity(glm::lvec2 dir);
	void SetVelocity(glm::lvec2 dir);

	void AddForce(std::int64_t duration, glm::lvec2 dir);

	void OverrideGravity(std::int64_t duration, std::int64_t value) { _gravityOverride = { duration, value }; }
	void OverrideGroundFriction(std::int64_t duration, std::int64_t value) { _groundFrictionOverride = { duration, value }; }
	void OverrideAirFriction(std::int64_t duration, std::int64_t value) { _airFrictionOverride = { duration, value }; }

	CharacterPhysicsManager(GameCharacter* owner, ServiceManager* serviceManager);
	~CharacterPhysicsManager();
};