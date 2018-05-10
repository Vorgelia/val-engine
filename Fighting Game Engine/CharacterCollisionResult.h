#pragma once
#include "AttackData.h"
#include "DefenceData.h"
#include <boost/optional/optional.hpp>

class GameCharacter;

struct AttackCollisionHit
{
public:
	AttackData hitbox;
	DefenceData hurtbox;

	AttackCollisionHit(const AttackData& hitbox, const DefenceData& hurtbox);
};

struct CollisionHit
{
	CollisionBox thisCollision;
	CollisionBox otherCollision;

	ve::vec2 depenetrationDistance;

	CollisionHit(const CollisionBox& thisCollision, const CollisionBox& otherCollision, const ve::vec2& depenetrationDistance);
};

enum class CharacterAttackResultFlags : unsigned char
{
	None = 0b0,
	AttackHit = 0b1,
	AttackReceived = 0b10,
	AttacksTraded = 0b11
};

struct CharacterCollisionResult
{
	GameCharacter* otherCharacter;

	boost::optional<AttackCollisionHit> attackReceived;
	boost::optional<AttackCollisionHit> attackHit;

	std::vector<CollisionHit> collisionHits;

	CharacterAttackResultFlags attackResultFlags() const;

	CharacterCollisionResult(GameCharacter* otherCharacter, const boost::optional<AttackCollisionHit>&& attackReceived, const boost::optional<AttackCollisionHit>&& attackHit);
	CharacterCollisionResult();
};
