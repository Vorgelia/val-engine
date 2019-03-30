#include "GameCharacter.h"
#include "Script.h"
#include "ScriptManager.h"
#include "ResourceManager.h"
#include "FightingStageBehaviour.h"
#include "CharacterStateComponent.h"
#include "FightingGameManager.h"
#include "CharacterPhysicsComponent.h"
#include "GamePlayer.h"
#include "CharacterState.h"
#include "CharacterFrame.h"
#include "GameCharacterData.h"
#include "GameInstance.h"
#include "CharacterEventComponent.h"
#include "Transform.h"

VE_OBJECT_DEFINITION(GameCharacter);

void GameCharacter::OnInit()
{
	_filesystem = _owningInstance->Filesystem();
	_scriptManager = _owningInstance->ScriptManager();
	_resource = _owningInstance->ResourceManager();

	_stateComponent = AddComponentOfType<CharacterStateComponent>().get();
	_physicsComponent = AddComponentOfType<CharacterPhysicsComponent>().get();
	_eventComponent = AddComponentOfType<CharacterEventComponent>().get();
}

void GameCharacter::OnDestroyed()
{
	
}

void GameCharacter::Deserialize(const json& j)
{
	std::string dataPath;
	if(JSON::TryGetMember<std::string>(j, "dataPath", dataPath))
	{
		json* dataJson = _resource->GetJsonData(dataPath);
		if(dataJson != nullptr)
		{
			_characterData = std::make_unique<GameCharacterData>(*dataJson);

			std::string scriptPath;
			if(JSON::TryGetMember(*dataJson, "characterScript", scriptPath))
			{
				_characterScript = _scriptManager->GetScript(scriptPath);
				_scriptManager->HandleScriptCharacterBindings(*this, _characterScript);
			}

			_stateComponent->CacheCharacterData();
		}
	}

	GameObject::Deserialize(j);
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
	-> std::optional<AttackCollisionHit>
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
						const CollisionBox relativeHitCollision = hitCollision.RelativeTo(attacker->GetWorldTransform().GetPosition(), _flipped);
						const CollisionBox relativeHurtCollision = hurtCollision.RelativeTo(defender->GetWorldTransform().GetPosition(), other->_flipped);
						if(relativeHitCollision.Overlaps(relativeHurtCollision))
						{
							return AttackCollisionHit(hitbox, hurtbox);
						}
					}
				}
			}
		}
		return std::optional<AttackCollisionHit>();
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

GameCharacter::GameCharacter()
	: _scriptManager(nullptr)
	, _filesystem(nullptr)
	, _resource(nullptr)
	, _initialized(false)
	, _playerOwner(nullptr)
	, _characterScript(nullptr)
	, _flipped(false)
	, _stateComponent(nullptr)
	, _physicsComponent(nullptr)
	, _eventComponent(nullptr)
{

}
