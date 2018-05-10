#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "ValEngine.h"
#include "MathIncludes.hpp"
#include "JSON.h"

struct GameCharacterPhysicsParams
{
	ve::dec_t gravity;
	ve::dec_t baseGroundFriction;
	ve::dec_t baseAirFriction;

	ve::dec_t mass;

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

	ve::vec2 _sizeMultiplier;

	std::vector<std::string> _statePaths;
	std::vector<std::string> _frameDataPaths;

	GameCharacterPhysicsParams _physicsParams;

	GameCharacterData(const json& j);
	GameCharacterData() = default;
	~GameCharacterData() = default;
};