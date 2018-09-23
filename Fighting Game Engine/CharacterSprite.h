#pragma once
#include <string>
#include "MathIncludes.hpp"
#include "JSON.h"

class CharacterSprite
{
	std::string _sprite;
	ve::ivec4 _pixelRect;
	ve::ivec2 _originOffset;

public:
	std::string sprite() const;
	ve::ivec4 pixelRect() const;
	ve::ivec2 originOffset() const;

	CharacterSprite(const json& j);
	~CharacterSprite();
};