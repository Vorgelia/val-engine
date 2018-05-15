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
			if(otherCharacter == thisCharacter)
			{
				continue;
			}

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

void FightingStageBehaviour::HandleAttackHit(GameCharacter* attacker, GameCharacter* attackReceiver, const AttackCollisionHit& hit, bool wasTrade)
{
	const std::vector<std::string>& flags = attackReceiver->eventComponent()->HandleAttackReceived(attacker, hit);
	if(wasTrade)
	{
		attacker->eventComponent()->HandleTradeSuccess(attackReceiver, hit, flags);
	}
	attacker->eventComponent()->HandleAttackHit(attackReceiver, hit, flags);
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
			case CharacterAttackResultFlags::AttackReceived:
			default:
				break;
			case CharacterAttackResultFlags::AttackHit:
				HandleAttackHit(character, result.otherCharacter, result.attackHit.get());
				break;
			case CharacterAttackResultFlags::AttacksTraded: 
				if(!tradeResolutionMap.insert_or_assign(character, std::unordered_set<GameCharacter*>()).first->second.emplace(result.otherCharacter).second)
				{
					continue;
				}

				bool thisCharacterTradePriorityFlag = character->eventComponent()->ResolveTrade(result.otherCharacter, result.attackHit.get(), result.attackReceived.get());
				bool otherCharacterTradePriorityFlag = result.otherCharacter->eventComponent()->ResolveTrade(character, result.attackReceived.get(), result.attackHit.get());

				if(thisCharacterTradePriorityFlag && otherCharacterTradePriorityFlag)
				{
					character->eventComponent()->HandleTradeUnresolved(result.otherCharacter, result.attackHit.get(), result.attackReceived.get());
					result.otherCharacter->eventComponent()->HandleTradeUnresolved(character, result.attackReceived.get(), result.attackHit.get());
				}
				else if(thisCharacterTradePriorityFlag)
				{
					HandleAttackHit(character, result.otherCharacter, result.attackHit.get(), true);
				}
				else if(!otherCharacterTradePriorityFlag)
				{
					HandleAttackHit(result.otherCharacter, character, result.attackHit.get(), true);
				}
				else
				{
					HandleAttackHit(character, result.otherCharacter, result.attackHit.get());
					HandleAttackHit(result.otherCharacter, character, result.attackReceived.get());
				}

				tradeResolutionMap.insert_or_assign(result.otherCharacter, std::unordered_set<GameCharacter*>()).first->second.emplace(character);
				break;
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
}

FightingStageBehaviour::FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j)
	: Behaviour(owner, serviceManager, j)
{
	JSON::TryGetMember(j, "stageBounds", _stageBounds);
	_gameManager = serviceManager->FightingGameManager();
}