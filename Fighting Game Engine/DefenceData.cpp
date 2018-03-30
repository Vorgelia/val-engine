#include "DefenceData.h"


float DefenceData::damageMultiplier() const
{
	return _damageMultiplier;
}

DefenceData::DefenceData(const json& j) : CharacterHitData(j)
{
	_damageMultiplier = JSON::Get<float>(j["damageMultiplier"]);
}

DefenceData::~DefenceData()
= default;
