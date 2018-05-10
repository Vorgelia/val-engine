#include "FightingStageBehaviour.h"
#include "ServiceManager.h"
#include "FightingGameManager.h"
#include "GameCharacter.h"
#include "CharacterStateComponent.h"
#include "CharacterFrame.h"
#include "GameCharacterData.h"
#include "CharacterEventComponent.h"
#include "CharacterPhysicsComponent.h"

VE_BEHAVIOUR_REGISTER_TYPE(FightingStageBehaviour);

FightingStageBehaviour::CharacterCollisionResultMap FightingStageBehaviour::GenerateCharacterCollisionResults() const
{
	std::unordered_set<GameCharacter*> handledCharacters;
	CharacterCollisionResultMap collisionResults;
	for(GameCharacter* thisCharacter : _gameManager->characters())
	{
		for(GameCharacter* otherCharacter : _gameManager->characters())
		{
			if(handledCharacters.find(otherCharacter) != handledCharacters.end())
			{
				continue;
			}

			CharacterCollisionResult collisionResult = thisCharacter->GenerateCollisions(otherCharacter);
			auto addResultLambda = [&](GameCharacter* character, const CharacterCollisionResult& result)
			{
				auto iter = collisionResults.find(character);
				if(iter == collisionResults.end())
				{
					iter = collisionResults.emplace(std::make_pair(character, std::vector<CharacterCollisionResult>())).first;
					iter->second.reserve(_gameManager->characters().size());
				}

				iter->second.emplace_back(result);
			};

			addResultLambda(thisCharacter, collisionResult);
			std::swap(collisionResult.attackHit, collisionResult.attackReceived);
			collisionResult.otherCharacter = thisCharacter;
			addResultLambda(otherCharacter, collisionResult);
		}

		handledCharacters.emplace(thisCharacter);
	}

	const auto sortLambda = [](const CharacterCollisionResult& result1, const CharacterCollisionResult& result2)->bool
	{
		return result1.otherCharacter->characterData()->_eventResolutionPriority > result2.otherCharacter->characterData()->_eventResolutionPriority;
	};

	for(auto& iter : collisionResults)
	{
		std::vector<CharacterCollisionResult>& results = iter.second;
		std::sort(results.begin(), results.end(), sortLambda);
	}

	return collisionResults;
}

const ve::vec4& FightingStageBehaviour::stageBounds() const
{
	return _stageBounds;
}

void FightingStageBehaviour::GameUpdate()
{
	for(auto& iter : _gameManager->characters())
	{
		iter->CharacterUpdate();
	}
}

void FightingStageBehaviour::LateGameUpdate()
{
	CharacterCollisionResultMap collisionResults = GenerateCharacterCollisionResults();

	std::unordered_map<GameCharacter*, std::unordered_set<GameCharacter*>> tradeResolutionMap;
	for(auto& iter : collisionResults)
	{
		GameCharacter* character = iter.first;
		std::vector<CharacterCollisionResult>& results = iter.second;

		for(const CharacterCollisionResult& result : results)
		{
			switch(result.attackResultFlags())
			{
			case CharacterAttackResultFlags::None: 
			default:
				return;
			case CharacterAttackResultFlags::AttackHit: break;
			case CharacterAttackResultFlags::AttackReceived: break;
			case CharacterAttackResultFlags::AttacksTraded: break;
			}
		}
	}

	for(auto& iter = collisionResults.rbegin() ; iter != collisionResults.rend(); ++iter)
	{
		GameCharacter* character = iter->first;
		std::vector<CharacterCollisionResult>& results = iter->second;

		for(const CharacterCollisionResult& result : results)
		{
			for(const CollisionHit hit : result.collisionHits)
			{
				character->physicsComponent()->HandleCharacterCollision(result.otherCharacter, hit);
			}
		}
	}

	for(auto& iter : _gameManager->characters())
	{
		iter->CharacterLateUpdate();
	}

	//after all events are handled, remove all characters that want to be removed this frame.
}

FightingStageBehaviour::FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j)
	: Behaviour(owner, serviceManager, j)
{
	JSON::TryGetMember(j, "stageBounds", _stageBounds);
	_gameManager = serviceManager->FightingGameManager();
}