#pragma once
#include <vector>
#include "JSON.h"

struct CollisionBox;

class CharacterCollisionData
{
	std::vector<CollisionBox> _collision;

public:
	const std::vector<CollisionBox>& collision() const;

	CharacterCollisionData(const json& j);
	virtual ~CharacterCollisionData() = default;
};