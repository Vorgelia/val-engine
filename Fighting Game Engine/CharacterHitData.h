#pragma once
#include <vector>
#include "JSON.h"

struct CollisionBox;

class CharacterHitData
{
	std::vector<CollisionBox> _collision;

public:
	const std::vector<CollisionBox>& collision() const;

	CharacterHitData(const json& j);
	virtual ~CharacterHitData() = default;
};