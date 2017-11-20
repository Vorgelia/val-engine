#pragma once
#include <string>
#include "MathIncludes.hpp"
#include "JSON.h"

class CharacterSprite
{
	std::string _sprite;
	glm::ivec4 _pixelRect;
	glm::ivec2 _originOffset;

public:
	std::string sprite() const;
	glm::ivec4 pixelRect() const;
	glm::ivec2 originOffset() const;

	CharacterSprite(const json& j);
	~CharacterSprite();
};