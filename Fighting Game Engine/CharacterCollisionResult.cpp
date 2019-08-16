#include "CharacterCollisionResult.h"
#include "SerializationProxy.h"
#include <optional>

void AttackCollisionHit::RegisterReflectionFields() const
{
	AddReflectionField(VE_REFLECTION_ARG(hitbox));
	AddReflectionField(VE_REFLECTION_ARG(hurtbox));
}

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
	CharacterAttackResultFlags flags = CharacterAttackResultFlags::None;
	if(attackReceived.has_value())
	{
		flags |= CharacterAttackResultFlags::AttackReceived;
	}

	if(attackHit.has_value())
	{
		flags |= CharacterAttackResultFlags::AttackHit;
	}

	return CharacterAttackResultFlags(flags);
}

CharacterCollisionResult::CharacterCollisionResult(
	GameCharacter* otherCharacter, const std::optional<AttackCollisionHit>&& attackReceived, const std::optional<AttackCollisionHit>&& attackHit)
	: otherCharacter(otherCharacter)
	, attackReceived(attackReceived)
	, attackHit(attackHit)
{
	
}