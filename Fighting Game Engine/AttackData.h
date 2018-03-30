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
	int damage() const;
	int hitstun() const;
	int blockstun() const;

	const std::vector<std::string>& flags() const;

	AttackData(const json& j);
	~AttackData() = default;
};