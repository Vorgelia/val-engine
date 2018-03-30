#include "AttackData.h"


int AttackData::damage() const
{
	return _damage;
}

int AttackData::hitstun() const
{
	return _hitstun;
}

int AttackData::blockstun() const
{
	return _blockstun;
}

const std::vector<std::string>& AttackData::flags() const
{
	return _flags;
}

AttackData::AttackData(const json& j) : CharacterHitData(j)
{
	_damage = JSON::Get<int>(j["damage"]);
	_hitstun = JSON::Get<int>(j["hitstun"]);
	_blockstun = JSON::Get<int>(j["blockstun"]);
}
