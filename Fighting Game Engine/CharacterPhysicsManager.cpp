#include "CharacterPhysicsManager.h"
#include "GameCharacter.h"
#include "GameCharacterData.h"
#include "Transform.h"
#include "ServiceManager.h"
#include "FightingStageBehaviour.h"
#include "FightingGameManager.h"

void CharacterPhysicsManager::Update()
{
	FightingStageBehaviour* stageBehaviour = _fightingGameManager->stageBehaviour();
	if(stageBehaviour == nullptr)
	{
		return;
	}

	_accumulatedOffset += _velocity;

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

	if(_grounded)
	{
		_accumulatedOffset.y = glm::max<glm::lvec2::value_type>(0, _accumulatedOffset.y);
	}

	Transform* transform = _owner->object()->transform();

	transform->position += _accumulatedOffset;
	transform->position.x = glm::clamp(transform->position.x, stageBehaviour->stageBounds().x, stageBehaviour->stageBounds().z);
	transform->position.y = glm::clamp(transform->position.y, stageBehaviour->stageBounds().y, stageBehaviour->stageBounds().w);

	_grounded = (transform->position.y == 0);

	if(_grounded)
	{
		const std::int64_t groundFriction = _groundFrictionOverride.duration > 0 ? _groundFrictionOverride.value : _owner->characterData()->_physicsParams.baseGroundFriction;
		_velocity.x = glm::moveTowards<glm::lvec2::value_type>(_velocity.x, 0, groundFriction);
		_velocity.y = 0;
	}
	else
	{
		const std::int64_t airFriction = _airFrictionOverride.duration > 0 ? _airFrictionOverride.value : _owner->characterData()->_physicsParams.baseAirFriction;
		const std::int64_t gravity = _gravityOverride.duration > 0 ? _gravityOverride.value : _owner->characterData()->_physicsParams.gravity;

		_velocity.x = glm::moveTowards<glm::lvec2::value_type>(_velocity.x, 0, airFriction);
		_velocity.y -= gravity;
	}

	_accumulatedOffset = glm::lvec2(0, 0);
	if(_groundFrictionOverride.duration > 0)
		--_groundFrictionOverride.duration;
	if(_airFrictionOverride.duration > 0)
		--_airFrictionOverride.duration;
	if(_gravityOverride.duration > 0)
		--_gravityOverride.duration;
}

void CharacterPhysicsManager::AddOffset(glm::lvec2 dir)
{
	_accumulatedOffset += dir;
}

void CharacterPhysicsManager::AddVelocity(glm::lvec2 dir)
{
	_velocity += dir;
}

void CharacterPhysicsManager::SetVelocity(glm::lvec2 dir)
{
	_velocity = dir;
}

glm::lvec2 CharacterPhysicsManager::GetVelocity() const
{
	return _velocity;
}

void CharacterPhysicsManager::AddForce(std::int64_t duration, glm::lvec2 dir)
{
	_forces.emplace_back(CharacterPhysicsForce{ duration, dir });
}

CharacterPhysicsManager::CharacterPhysicsManager(GameCharacter * owner, ServiceManager * serviceManager)
	: _owner(owner)
	, _grounded(false)
{
	_fightingGameManager = serviceManager->FightingGameManager();
}

CharacterPhysicsManager::~CharacterPhysicsManager()
= default;
