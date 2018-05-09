#pragma once
#include "MathIncludes.hpp"


struct CollisionBox
{
	glm::lvec2 center;
	glm::lvec2 extents;
	glm::lvec2 pivotOffset;

	glm::lvec2 pivotedCenter() const;

	void AxisMinMax(glm::lvec2& out_minMaxX, glm::lvec2& out_minMaxY) const;

	glm::lvec2 DepenetrationDistance(const CollisionBox& other) const;
	bool Overlaps(const CollisionBox& other) const;

	CollisionBox operator+(const glm::lvec2& rhs) const;

	CollisionBox RelativeTo(const glm::lvec2& position, bool flipped = false) const;
	static CollisionBox FromTopLeft(const glm::lvec2& topLeft, const glm::lvec2& size);

	CollisionBox(const glm::lvec2& center, const glm::lvec2& extents, const glm::lvec2& pivotOffset = glm::lvec2(0,0));
	~CollisionBox() = default;
};