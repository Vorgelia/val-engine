#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "MathIncludes.hpp"
#include "JSON.h"

struct GameCharacterPhysicsParams
{
	int gravity;
	int baseGroundFriction;
	int baseAirFriction;

	GameCharacterPhysicsParams(const json& j);
	GameCharacterPhysicsParams() = default;
	~GameCharacterPhysicsParams() = default;
};

struct GameCharacterData
{
	std::string _name;
	std::string _characterScript;
	std::unordered_set<std::string> _flags;

	glm::ivec2 _sizeMultiplier;

	std::vector<std::string> _statePaths;
	std::vector<std::string> _frameDataPaths;

	GameCharacterPhysicsParams _physicsParams;

	GameCharacterData(const json& j);
	GameCharacterData() = default;
	~GameCharacterData() = default;
};