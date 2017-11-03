#pragma once
#include <vector>
#include <string>
#include "AttackData.h"


class CharacterFrame
{
	std::string _id;

	std::vector<CollisionBox> _hitboxes;
	std::vector<CollisionBox> _hurtboxes;
	std::vector<CollisionBox> _collision;

	int damage;
	int hitstun;
	int blockstun;
public:

	//Hitboxes
	//Hurtboxes

	CharacterFrame();
	~CharacterFrame();
};