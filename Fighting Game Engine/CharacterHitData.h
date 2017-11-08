#pragma once
#include <vector>
#include "JSON.h"

class CollisionBox;

class CharacterHitData
{
	std::vector<CollisionBox> _collision;

public:
	const std::vector<CollisionBox>& collision();

	CharacterHitData(const json& j);
	~CharacterHitData();
};