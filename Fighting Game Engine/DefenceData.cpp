#include "DefenceData.h"


float DefenceData::damageMultiplier()
{
	return _damageMultiplier;
}

DefenceData::DefenceData(const json& j) : CharacterHitData(j)
{
	_damageMultiplier = JSON::Get<float>(j["damageMultiplier"]);
}

DefenceData::~DefenceData()
{

}
