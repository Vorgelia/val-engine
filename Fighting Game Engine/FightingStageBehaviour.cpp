#include "FightingStageBehaviour.h"
#include "ServiceManager.h"
#include "FightingGameManager.h"
#include "GameCharacter.h"
#include "CharacterStateComponent.h"
#include "CharacterFrame.h"
#include "GameCharacterData.h"
#include "CharacterEventComponent.h"

VE_BEHAVIOUR_REGISTER_TYPE(FightingStageBehaviour);

std::unordered_map<GameCharacter*, std::vector<CharacterCollisionResult>> FightingStageBehaviour::GenerateCharacterCollisionResults() const
{
	std::unordered_set<GameCharacter*> handledCharacters;
	std::unordered_map<GameCharacter*, std::vector<CharacterCollisionResult>> collisionResults;
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

const glm::lvec4& FightingStageBehaviour::stageBounds() const
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
	std::unordered_map<GameCharacter*, std::vector<CharacterCollisionResult>> collisionResults = GenerateCharacterCollisionResults();

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
				break;
			case CharacterAttackResultFlags::AttackHit:
				//character->eventComponent()->HandleAttackHit();
				break;
			case CharacterAttackResultFlags::AttackReceived: 
				//character->eventComponent()->HandleAttackReceived();
				break;
			case CharacterAttackResultFlags::AttacksTraded: 
				//character->eventComponent()->HandleAttackTraded();
				break;
			}
		}
	}

	//after all events are handled, remove all characters that want to be removed this frame.
}

FightingStageBehaviour::FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j)
	: Behaviour(owner, serviceManager, j)
{
	JSON::TryGetMember(j, "stageBounds", _stageBounds);
	_gameManager = serviceManager->FightingGameManager();
}