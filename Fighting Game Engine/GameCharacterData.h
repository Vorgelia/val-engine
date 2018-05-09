#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "MathIncludes.hpp"
#include "JSON.h"

struct GameCharacterPhysicsParams
{
	std::int64_t gravity;
	std::int64_t baseGroundFriction;
	std::int64_t baseAirFriction;

	std::int64_t mass;

	GameCharacterPhysicsParams(const json& j);
	GameCharacterPhysicsParams() = default;
	~GameCharacterPhysicsParams() = default;
};

struct GameCharacterData
{
	std::string _name;
	std::string _characterScript;
	std::unordered_set<std::string> _flags;
	int _eventResolutionPriority;

	glm::ivec2 _sizeMultiplier;

	std::vector<std::string> _statePaths;
	std::vector<std::string> _frameDataPaths;

	GameCharacterPhysicsParams _physicsParams;

	GameCharacterData(const json& j);
	GameCharacterData() = default;
	~GameCharacterData() = default;
};