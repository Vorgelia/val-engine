#include "AttackData.h"


int AttackData::damage()
{
	return _damage;
}

int AttackData::hitstun()
{
	return _hitstun;
}

int AttackData::blockstun()
{
	return _blockstun;
}

const std::vector<std::string>& AttackData::flags()
{
	return _flags;
}

AttackData::AttackData(const json& j) : CharacterHitData(j)
{
	_damage = JSON::Get<int>(j["damage"]);
	_hitstun = JSON::Get<int>(j["hitstun"]);
	_blockstun = JSON::Get<int>(j["blockstun"]);
}


AttackData::~AttackData()
{
}
