#pragma once
#include <vector>
#include "CharacterCollisionData.h"
#include <unordered_set>
#include "IReflectable.h"

class CharacterHitData :
	public CharacterCollisionData, public IReflectable
{
protected:
	//hitstun, blockstun, chip damage, pushback
	json _data;

	int _sequenceID;

	//projectile, high, unblockable, throw
	std::unordered_set<std::string> _flags;

	std::vector<std::string> _flagCollisionRequireMask;
	std::vector<std::string> _flagCollisionIgnoreMask;
	//mask for ignoring collision

	bool CanCollideWith_Impl(const CharacterHitData& other) const;

	virtual void RegisterReflectionFields() const override;

public:
	const json& data() const { return _data; }
	int sequenceID() const { return _sequenceID; }
	const std::unordered_set<std::string>& flags() const { return _flags; }

	bool CanCollideWith(const CharacterHitData& other) const;

	CharacterHitData(const json& j);
	~CharacterHitData() = default;
};
