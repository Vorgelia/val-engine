#include "CharacterPhysicsManager.h"
#include "GameCharacter.h"
#include "GameCharacterData.h"
#include "Transform.h"

void CharacterPhysicsManager::Update()
{
	_accumulatedOffset += _velocity;
	if (_grounded)
	{
		_accumulatedOffset.y = glm::max<glm::lvec2::value_type>(0, _accumulatedOffset.y);
	}

	Transform* transform = _owner->object()->transform();

	transform->position += _accumulatedOffset;
	transform->position.y = glm::max<glm::lvec2::value_type>(0, transform->position.y);
	
	_grounded = (transform->position.y == 0);

	if (_grounded)
	{
		_velocity.x = glm::moveTowards<glm::lvec2::value_type>(_velocity.x, 0, _owner->characterData()->_physicsParams.baseGroundFriction);
		_velocity.y = 0;
	}
	else
	{
		_velocity.x = glm::moveTowards<glm::lvec2::value_type>(_velocity.x, 0, _owner->characterData()->_physicsParams.baseAirFriction);
		_velocity.y -= _owner->characterData()->_physicsParams.gravity;
	}

	_accumulatedOffset = glm::lvec2(0, 0);
}

void CharacterPhysicsManager::AddOffset(glm::lvec2::value_type x, glm::lvec2::value_type y)
{
	_accumulatedOffset += glm::lvec2(x, y);
}

void CharacterPhysicsManager::AddVelocity(glm::lvec2::value_type x, glm::lvec2::value_type y)
{
	_velocity += glm::lvec2(x, y);
}

void CharacterPhysicsManager::SetVelocity(glm::lvec2::value_type x, glm::lvec2::value_type y)
{
	_velocity += glm::lvec2(x, y);
}

CharacterPhysicsManager::CharacterPhysicsManager(GameCharacter * owner, ServiceManager * serviceManager)
	: _owner(owner)
{
	
}

CharacterPhysicsManager::~CharacterPhysicsManager()
= default;
