#pragma once
#include <GLM\glm.hpp>
#include "JSON.h"

class CollisionBox
{
public:
	glm::ivec4 rect;//Center X, Center Y, Width/2, Height/2

	CollisionBox flipped();

	bool Overlaps(CollisionBox* hitbox);

	CollisionBox operator+(const glm::ivec2& rhs);

	static CollisionBox FromTopLeft(glm::ivec4 rect);
	CollisionBox(glm::ivec4 rect);
};