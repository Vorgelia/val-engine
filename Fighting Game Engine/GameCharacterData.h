#pragma once
#include <string>
#include <vector>
#include <unordered_set>
#include "MathIncludes.hpp"
#include "JSON.h"

struct GameCharacterData
{
private:
	std::string _name;
	std::string _characterScript;
	std::unordered_set<std::string> _flags;

	glm::ivec2 _sizeMultiplier;

	std::vector<std::string> _statePaths;
	std::vector<std::string> _frameDataPaths;
public:

	GameCharacterData(const json& j);
	GameCharacterData() = default;
	~GameCharacterData();
};