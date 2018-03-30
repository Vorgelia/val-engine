#pragma once
#include <GLM\glm.hpp>
#include "JSON.h"

class CollisionBox
{
public:
	glm::ivec4 rect;//Center X, Center Y, Width/2, Height/2

	CollisionBox flipped() const;

	bool Overlaps(CollisionBox* hitbox) const;

	CollisionBox operator+(const glm::ivec2& rhs) const;

	static CollisionBox FromTopLeft(glm::ivec4 rect);
	CollisionBox(glm::ivec4 rect);
};