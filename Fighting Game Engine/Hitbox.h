#pragma once
#include "CommonUtilIncludes.hpp"
class Hitbox
{
public:
	glm::ivec4 rect;//X,Y,Width,Height

	bool Overlap(Hitbox* hitbox);
	Hitbox(glm::ivec4 rect);
};

