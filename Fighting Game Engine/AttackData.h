#pragma once
#include <vector>
#include "CollisionBox.h"

class AttackData
{
public:
	std::vector<CollisionBox> hitboxes;
	AttackData();
	~AttackData();
};

