#pragma once
#include <vector>
#include "CharacterHitData.h"

class AttackData :
	public CharacterHitData
{
	int _damage;
	int _hitstun;
	int _blockstun;

	//projectile, high, unblockable, throw
	std::vector<std::string> _flags;

public:
	int damage();
	int hitstun();
	int blockstun();

	const std::vector<std::string>& flags();

	AttackData(const json& j);
	~AttackData();
};