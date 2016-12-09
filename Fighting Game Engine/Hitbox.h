#pragma once
#include "CommonUtilIncludes.hpp"
class Hitbox
{
public:
	glm::vec4 rect;//Center X, Center Y, Width/2, Height/2
	bool Overlap(Hitbox* hitbox);
	Hitbox flipped();
	
	Hitbox operator+(const glm::vec2 rhs);

	static Hitbox FromTopLeft(glm::vec4 rect);
	Hitbox(glm::vec4 rect);
};