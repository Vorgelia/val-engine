#pragma once
#include "CommonUtilIncludes.hpp"
class CollisionBox
{
public:
	glm::vec4 rect;//Center X, Center Y, Width/2, Height/2
	bool Overlap(CollisionBox* hitbox);
	CollisionBox flipped();

	CollisionBox operator+(const glm::vec2 rhs);

	static CollisionBox FromTopLeft(glm::vec4 rect);
	CollisionBox(glm::vec4 rect);
};