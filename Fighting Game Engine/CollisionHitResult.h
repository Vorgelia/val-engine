#pragma once
#include "MathIncludes.hpp"

struct CollisionHitResult
{
public:
	glm::lvec2 depenetrationDistance;

	CollisionHitResult();
	~CollisionHitResult() = default;
};

