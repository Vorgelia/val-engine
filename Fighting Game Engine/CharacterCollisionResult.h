#pragma once
#include "IReflectable.h"
#include "CharacterHitData.h"
#include "CollisionBox.h"
#include "EnumUtils.h"
#include <boost/optional/optional.hpp>
#include <optional>

class GameCharacter;

struct AttackCollisionHit : public IReflectable
{
public:
	CharacterHitData hitbox;
	CharacterHitData hurtbox;

	virtual void RegisterReflectionFields() const override;

	AttackCollisionHit(const CharacterHitData& hitbox, const CharacterHitData& hurtbox);
};

struct CollisionHit
{
	CollisionBox thisCollision;
	CollisionBox otherCollision;

	ve::vec2 depenetrationDistance;

	CollisionHit(const CollisionBox& thisCollision, const CollisionBox& otherCollision, const ve::vec2& depenetrationDistance);
};

enum class CharacterAttackResultFlags : std::uint8_t
{
	VE_BITMASK_VALUE_NONE,
	AttackHit = 0b1,
	AttackReceived = 0b10,
	AttacksTraded = 0b11,
	VE_BITMASK_VALUE_ALL
};

VE_DECLARE_BITMASK_ENUM(CharacterAttackResultFlags);

struct CharacterCollisionResult 
{
	GameCharacter* otherCharacter;

	std::optional<AttackCollisionHit> attackReceived;
	std::optional<AttackCollisionHit> attackHit;

	//TODO: +Arbitrary named hit data box hits
	std::vector<CollisionHit> collisionHits;

	CharacterAttackResultFlags attackResultFlags() const;

	CharacterCollisionResult(GameCharacter* otherCharacter, const std::optional<AttackCollisionHit>&& attackReceived, const std::optional<AttackCollisionHit>&& attackHit);
	CharacterCollisionResult() = default;
};
