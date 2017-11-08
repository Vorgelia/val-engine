#pragma once
#include "CharacterHitData.h"

class DefenceData :
	public CharacterHitData
{
	float _damageMultiplier;

public:
	float damageMultiplier();

	DefenceData(const json& j);
	~DefenceData();
};