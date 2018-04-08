#include "CollisionBox.h"
#include <GLM/detail/type_mat.hpp>
#include <GLM/detail/type_mat.hpp>

void CollisionBox::AxisMinMax(glm::lvec2& out_minMaxX, glm::lvec2& out_minMaxY) const
{
	out_minMaxX = glm::lvec2(center.x - extents.x, center.x + extents.x);
	out_minMaxY = glm::lvec2(center.y - extents.y, center.y + extents.y);
}

bool CollisionBox::Overlaps(const CollisionBox& other) const
{
	return (abs(center.x - other.center.x) < extents.y + other.extents.y) && (abs(center.y - other.center.y) < extents.y + other.extents.y);
}

glm::lvec2 CollisionBox::DepenetrationDistance(const CollisionBox& other) const
{
	if(!Overlaps(other))
	{
		return glm::lvec2(0, 0);
	}
	
	const glm::lvec2 centerDist = center - other.center;
	const glm::lvec2 centerDistSign = glm::nonZeroSign(centerDist);

	const glm::lvec2 distRemaining = other.extents + extents - glm::abs(centerDist);

	return glm::lvec2(distRemaining.x * centerDistSign.x, distRemaining.y * centerDistSign.y);
}

CollisionBox CollisionBox::FromTopLeft(const glm::lvec2& topLeft, const glm::lvec2& size)
{
	return CollisionBox(
		topLeft + size / std::int64_t(2),
		size / std::int64_t(2));
};

CollisionBox CollisionBox::operator+(const glm::lvec2& rhs) const
{
	return CollisionBox(center + rhs, extents);
}

CollisionBox::CollisionBox(const glm::lvec2& center, const glm::lvec2& extents)
	: center(center)
	, extents(extents)
{
	this->extents = glm::abs(extents);
}