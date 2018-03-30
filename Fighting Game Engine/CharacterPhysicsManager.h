#pragma once
#include "MathIncludes.hpp"

class ServiceManager;

class CharacterPhysicsManager
{
	friend class GameCharacter;
private:
	GameCharacter* _owner;

private:
	glm::lvec2 _accumulatedOffset;
	glm::lvec2 _velocity;

	bool _grounded;

	void Update();

public:
	void AddOffset(glm::lvec2::value_type x, glm::lvec2::value_type y);
	void AddVelocity(glm::lvec2::value_type x, glm::lvec2::value_type y);
	void SetVelocity(glm::lvec2::value_type x, glm::lvec2::value_type y);

	CharacterPhysicsManager(GameCharacter* owner, ServiceManager* serviceManager);
	~CharacterPhysicsManager();
};