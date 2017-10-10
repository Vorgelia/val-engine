#pragma once
#include <vector>
#include <string>
#include "AttackData.h"


class CharacterFrame
{
	std::string _id;
	std::vector<AttackData> _hitboxes;
public:

	//Hitboxes
	//Hurtboxes

	CharacterFrame();
	~CharacterFrame();
};