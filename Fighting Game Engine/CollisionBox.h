#pragma once
#include "MathIncludes.hpp"
#include "JSON.h"

class CollisionBox
{
public:
	glm::lvec4 rect;//Center X, Center Y, Width/2, Height/2

	CollisionBox flipped() const;

	bool Overlaps(CollisionBox* hitbox) const;

	CollisionBox operator+(const glm::lvec2& rhs) const;

	static CollisionBox FromTopLeft(glm::lvec4 rect);
	CollisionBox(glm::lvec4 rect);
};