#pragma once
#include "CharacterHitData.h"

class DefenceData :
	public CharacterHitData
{
	float _damageMultiplier;

public:
	float damageMultiplier() const;

	DefenceData(const json& j);
	~DefenceData();
};