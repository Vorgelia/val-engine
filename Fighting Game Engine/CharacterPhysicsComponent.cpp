#include "CharacterPhysicsComponent.h"
#include "GameCharacter.h"
#include "GameCharacterData.h"
#include "Transform.h"
#include "ServiceManager.h"
#include "FightingStageBehaviour.h"
#include "FightingGameManager.h"

void CharacterPhysicsComponent::Init()
{
}

void CharacterPhysicsComponent::Update()
{
	_accumulatedOffset += _velocity;

	UpdateForces();
	ApplyAccumulatedOffset();
	ApplyFriction();
	
	_accumulatedOffset = ve::vec2(0, 0);
}

void CharacterPhysicsComponent::LateUpdate()
{
	ApplyAccumulatedOffset();
}

void CharacterPhysicsComponent::ApplyAccumulatedOffset()
{
	FightingStageBehaviour* stageBehaviour = _fightingGameManager->stageBehaviour();
	if(stageBehaviour == nullptr)
	{
		return;
	}

	if(_grounded)
	{
		_accumulatedOffset.y = glm::max<ve::dec_t>(ve::dec_t(0), _accumulatedOffset.y);
	}

	Transform* transform = _owner->object()->transform();

	transform->position += _accumulatedOffset;
	transform->position.x = glm::clamp(transform->position.x, stageBehaviour->stageBounds().x, stageBehaviour->stageBounds().z);
	transform->position.y = glm::clamp(transform->position.y, stageBehaviour->stageBounds().y, stageBehaviour->stageBounds().w);

	_grounded = (transform->position.y == stageBehaviour->stageBounds().y);
}

void CharacterPhysicsComponent::UpdateForces()
{
	for(size_t i = 0; i < _forces.size(); )
	{
		if(_forces[i].duration > 0)
		{
			_accumulatedOffset += _forces[i].force;
			if(--_forces[i].duration <= 0)
			{
				_velocity += _forces[i].force;
			}
			++i;
		}
		else
		{
			std::swap(_forces.begin() + i, _forces.end() - 1);
			_forces.pop_back();
		}
	}
}

void CharacterPhysicsComponent::ApplyFriction()
{
	if(_grounded)
	{
		const ve::dec_t groundFriction = _groundFrictionOverride.duration > 0 ? _groundFrictionOverride.value : _owner->characterData()->_physicsParams.baseGroundFriction;
		_velocity.x = glm::moveTowards<ve::dec_t>(_velocity.x, ve::dec_t(0), groundFriction);
		_velocity.y = ve::dec_t(0);
	}
	else
	{
		const ve::dec_t airFriction = _airFrictionOverride.duration > 0 ? _airFrictionOverride.value : _owner->characterData()->_physicsParams.baseAirFriction;
		const ve::dec_t gravity = _gravityOverride.duration > 0 ? _gravityOverride.value : _owner->characterData()->_physicsParams.gravity;

		_velocity.x = glm::moveTowards<ve::dec_t>(_velocity.x, ve::dec_t(0), airFriction);
		_velocity.y -= gravity;
	}

	if(_groundFrictionOverride.duration > 0)
		--_groundFrictionOverride.duration;
	if(_airFrictionOverride.duration > 0)
		--_airFrictionOverride.duration;
	if(_gravityOverride.duration > 0)
		--_gravityOverride.duration;
}

void CharacterPhysicsComponent::AddOffset(ve::vec2 dir)
{
	_accumulatedOffset += dir;
}

void CharacterPhysicsComponent::AddVelocity(ve::vec2 dir)
{
	_velocity += dir;
}

void CharacterPhysicsComponent::SetVelocity(ve::vec2 dir)
{
	_velocity = dir;
}

ve::vec2 CharacterPhysicsComponent::GetVelocity() const
{
	return _velocity;
}

void CharacterPhysicsComponent::AddForce(ve::int_t duration, ve::vec2 dir)
{
	_forces.emplace_back(CharacterPhysicsForce{ duration, dir });
}

void CharacterPhysicsComponent::HandleCharacterCollision(const GameCharacter* other, const CollisionHit collision)
{
	//if (owner in corner and offset is in direction of corner)
		//return;

	ve::dec_t displacementPercentage;
	//if (other in corner and offset is in direction of corner)
		//displacementPercentage = ve::dec_t(1);
	//else
	{
		const ve::dec_t thisMass = _owner->characterData()->_physicsParams.mass;
		const ve::dec_t otherMass = other->characterData()->_physicsParams.mass;

		displacementPercentage = thisMass / (thisMass + otherMass);
	}
	
	 _accumulatedOffset.x += collision.depenetrationDistance.x * displacementPercentage;
}

CharacterPhysicsComponent::CharacterPhysicsComponent(GameCharacter * owner, ServiceManager * serviceManager)
	: GameCharacterComponent(owner, serviceManager)
	, _grounded(true)
{
	_fightingGameManager = serviceManager->FightingGameManager();
}
