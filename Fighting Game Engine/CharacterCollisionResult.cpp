#include "CharacterCollisionResult.h"

AttackCollisionHit::AttackCollisionHit(const CharacterHitData& hitbox, const CharacterHitData& hurtbox)
	: hitbox(hitbox)
	, hurtbox(hurtbox)
{
}

CollisionHit::CollisionHit(const CollisionBox& thisCollision, const CollisionBox& otherCollision, const ve::vec2& depenetrationDistance)
	: thisCollision(thisCollision)
	, otherCollision(otherCollision)
	, depenetrationDistance(depenetrationDistance)
{
}

//TODO: Templated enum flags type
CharacterAttackResultFlags CharacterCollisionResult::attackResultFlags() const
{
	unsigned char flags = unsigned char(CharacterAttackResultFlags::None);
	if(attackReceived.is_initialized())
	{
		flags += flags | unsigned char(CharacterAttackResultFlags::AttackReceived);
	}

	if(attackHit.is_initialized())
	{
		flags += flags | unsigned char(CharacterAttackResultFlags::AttackHit);
	}

	return CharacterAttackResultFlags(flags);
}

CharacterCollisionResult::CharacterCollisionResult(
	GameCharacter* otherCharacter, const boost::optional<AttackCollisionHit>&& attackReceived, const boost::optional<AttackCollisionHit>&& attackHit)
	: otherCharacter(otherCharacter)
	, attackReceived(attackReceived)
	, attackHit(attackHit)
{
	
}

CharacterCollisionResult::CharacterCollisionResult()
	: attackReceived()
	, attackHit()
	, collisionHits()
{
}
