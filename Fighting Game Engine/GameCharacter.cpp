#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "CharacterStateComponent.h"
#include "CharacterPhysicsComponent.h"
#include "GamePlayer.h"
#include "CharacterState.h"
#include "CharacterFrame.h"
#include "GameCharacterData.h"
#include "GameInstance.h"
#include "CharacterEventComponent.h"
#include "Transform.h"

VE_BEHAVIOUR_REGISTER_TYPE(GameCharacter);

const GameCharacterData* GameCharacter::characterData() const
{
	return _characterData.get();
}

CharacterStateComponent* GameCharacter::stateComponent() const
{
	return _stateComponent.get();
}

CharacterPhysicsComponent* GameCharacter::physicsComponent() const
{
	return _physicsComponent.get();
}

CharacterEventComponent * GameCharacter::eventComponent() const
{
	return _eventComponent.get();
}

void GameCharacter::SetOwner(GamePlayer* owner)
{
	_playerOwner = owner;
}

CharacterCollisionResult GameCharacter::GenerateCollisions(const GameCharacter* other) const
{
	const CharacterFrame* otherFrame = other->stateComponent()->currentFrame();
	const CharacterFrame* thisFrame = stateComponent()->currentFrame();

	CharacterCollisionResult collisionResult;
	collisionResult.otherCharacter = const_cast<GameCharacter*>(other);

	auto& hitResultPicker = [&](const GameCharacter* attacker, const GameCharacter* defender, const std::vector<CharacterHitData>& hitboxes, const std::vector<CharacterHitData>& hurtboxes)
	-> boost::optional<AttackCollisionHit>
	{
		for(auto& hitbox : hitboxes)
		{
			for(auto& hurtbox : hurtboxes)
			{
				if(attacker->stateComponent()->_usedHitboxSequenceIDs.count(hitbox.sequenceID()) > 0 
					|| !hitbox.CanCollideWith(hurtbox))
				{
					continue;
				}

				for(auto& hitCollision : hitbox.collision())
				{
					for(auto& hurtCollision : hurtbox.collision())
					{
						const CollisionBox relativeHitCollision = hitCollision.RelativeTo(attacker->object()->transform()->position, _flipped);
						const CollisionBox relativeHurtCollision = hurtCollision.RelativeTo(defender->object()->transform()->position, other->_flipped);
						if(relativeHitCollision.Overlaps(relativeHurtCollision))
						{
							return AttackCollisionHit(hitbox, hurtbox);
						}
					}
				}
			}
		}
		return boost::optional<AttackCollisionHit>();
	};
	
	collisionResult.attackReceived = hitResultPicker(other, this, otherFrame->hitboxes(), thisFrame->hurtboxes());
	collisionResult.attackHit = hitResultPicker(this, other, thisFrame->hitboxes(), otherFrame->hurtboxes());
	
	for(const CollisionBox& thisCollision : thisFrame->collision())
	{
		for(const CollisionBox& otherCollision : otherFrame->collision())
		{
			const ve::vec2 depenetrationDist = thisCollision.DepenetrationDistance(otherCollision);
			if(depenetrationDist != ve::vec2(0,0))
			{
				collisionResult.collisionHits.emplace_back(CollisionHit(thisCollision, otherCollision, depenetrationDist));
			}
		}
	}

	return collisionResult;
}

void GameCharacter::CharacterInit()
{
	if(_characterScript != nullptr && _characterScript->HasFunction("CharacterInit"))
	{
		_characterScript->ExecuteFunction("CharacterInit");
	}
	_initialized = true;
}

void GameCharacter::CharacterUpdate()
{
	if(!_initialized)
	{
		CharacterInit();
	}

	_stateComponent->EvaluateNextState();

	if(_characterScript != nullptr)
	{
		_characterScript->ExecuteFunction("CharacterUpdate");
	}

	_stateComponent->Update();
	_physicsComponent->Update();
}

void GameCharacter::CharacterLateUpdate()
{
	_physicsComponent->LateUpdate();
	UpdateSystemFlags();
}

void GameCharacter::UpdateSystemFlags()
{
	_systemFlags.clear();
}

GameCharacter::GameCharacter(Object* owner, GameInstance* serviceManager, const json& j) : Behaviour(owner, serviceManager, j)
{
	_filesystem = serviceManager->Filesystem();
	_scriptManager = serviceManager->ScriptManager();
	_resource = serviceManager->ResourceManager();

	_initialized = false;

	std::string dataPath;
	if(JSON::TryGetMember<std::string>(j, "dataPath", dataPath))
	{
		json* dataJson = _resource->GetJsonData(dataPath);
		if(dataJson != nullptr)
		{
			_characterData = std::make_unique<GameCharacterData>(*dataJson);
			_stateComponent = std::make_unique<CharacterStateComponent>(this, _serviceManager);
			_physicsComponent = std::make_unique<CharacterPhysicsComponent>(this, _serviceManager);
			_eventComponent = std::make_unique<CharacterEventComponent>(this, _serviceManager);

			std::string scriptPath;
			if(JSON::TryGetMember(*dataJson, "characterScript", scriptPath))
			{
				_characterScript = _scriptManager->GetScript(scriptPath);
				_scriptManager->HandleScriptCharacterBindings(*this, _characterScript);
			}

			_stateComponent->Init();
			_physicsComponent->Init();
			_eventComponent->Init();
		}
	}
}

bool CharacterSortingPredicate::operator()(const GameCharacter* lhs, const GameCharacter* rhs) const
{
	if(lhs->characterData() == nullptr)
	{
		return rhs;
	}

	if(rhs->characterData() == nullptr)
	{
		return lhs;
	}
	return lhs->characterData()->_eventResolutionPriority > rhs->characterData()->_eventResolutionPriority;
}
