#include "CollisionBox.h"
#include <GLM/detail/type_mat.hpp>

ve::vec2 CollisionBox::pivotedCenter() const
{
	return center + pivotOffset;
}

void CollisionBox::AxisMinMax(ve::vec2& out_minMaxX, ve::vec2& out_minMaxY) const
{
	out_minMaxX = ve::vec2(center.x - extents.x, center.x + extents.x);
	out_minMaxY = ve::vec2(center.y - extents.y, center.y + extents.y);
}

bool CollisionBox::Overlaps(const CollisionBox& other) const
{
	return (ve::dec_t::Abs(center.x - other.center.x) < extents.y + other.extents.y) && (ve::dec_t::Abs(center.y - other.center.y) < extents.y + other.extents.y);
}

ve::vec2 CollisionBox::DepenetrationDistance(const CollisionBox& other) const
{
	if(!Overlaps(other))
	{
		return ve::vec2(0, 0);
	}

	const ve::vec2 displacementSign = glm::nonZeroSign(pivotedCenter() - other.pivotedCenter());
	const ve::vec2 desiredLocation = other.center + (other.extents + extents) * displacementSign;

	return desiredLocation - center;
}

CollisionBox CollisionBox::RelativeTo(const ve::vec2& position, bool flipped) const
{
	ve::vec2 newPosition = position;
	ve::vec2 newPivotOffset = pivotOffset;

	if(flipped)
	{
		newPosition.x = -newPosition.x;
		newPivotOffset.x = -newPivotOffset.x;
	}

	return CollisionBox(center + newPosition, extents, newPivotOffset);
}

CollisionBox CollisionBox::FromTopLeft(const ve::vec2& topLeft, const ve::vec2& size)
{
	return CollisionBox(
		topLeft + size / ve::dec_t(2),
		size / ve::dec_t(2));
};

CollisionBox CollisionBox::operator+(const ve::vec2& rhs) const
{
	return CollisionBox(center + rhs, extents);
}

CollisionBox::CollisionBox(const ve::vec2& center, const ve::vec2& extents, const ve::vec2& pivotOffset)
	: center(center)
	, extents(glm::abs(extents))
	, pivotOffset(pivotOffset)
{
}