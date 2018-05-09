#include "CollisionBox.h"
#include <GLM/detail/type_mat.hpp>

glm::lvec2 CollisionBox::pivotedCenter() const
{
	return center + pivotOffset;
}

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
	//TODO: Split this into AABB and non-stacking AABB
	const glm::lvec2 displacementSign = glm::nonZeroSign(pivotedCenter() - other.pivotedCenter());
	const glm::lvec2 desiredLocation = other.center + glm::lvec2(other.extents.x + extents.x, 0) * displacementSign;

	return desiredLocation - center;
}

CollisionBox CollisionBox::RelativeTo(const glm::lvec2& position, bool flipped) const
{
	return CollisionBox(center + (flipped ? -position : position), extents);
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

CollisionBox::CollisionBox(const glm::lvec2& center, const glm::lvec2& extents, const glm::lvec2& pivotOffset)
	: center(center)
	, extents(glm::abs(extents))
	, pivotOffset(pivotOffset)
{
}