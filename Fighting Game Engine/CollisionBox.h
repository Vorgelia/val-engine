#pragma once
#include "ValEngine.h"
#include "MathIncludes.hpp"


struct CollisionBox
{
	ve::vec2 center;
	ve::vec2 extents;
	ve::vec2 pivotOffset;

	ve::vec2 pivotedCenter() const;

	void AxisMinMax(ve::vec2& out_minMaxX, ve::vec2& out_minMaxY) const;

	ve::vec2 DepenetrationDistance(const CollisionBox& other) const;
	bool Overlaps(const CollisionBox& other) const;

	CollisionBox operator+(const ve::vec2& rhs) const;

	CollisionBox RelativeTo(const ve::vec2& position, bool flipped = false) const;
	static CollisionBox FromTopLeft(const ve::vec2& topLeft, const ve::vec2& size);

	CollisionBox(const ve::vec2& center, const ve::vec2& extents, const ve::vec2& pivotOffset = ve::vec2(0,0));
	~CollisionBox() = default;
};