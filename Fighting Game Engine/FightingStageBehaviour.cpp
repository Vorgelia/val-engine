#include "FightingStageBehaviour.h"
#include "ServiceManager.h"
#include "FightingGameManager.h"
#include "GameCharacter.h"
#include "CharacterStateComponent.h"
#include "CharacterFrame.h"

VE_BEHAVIOUR_REGISTER_TYPE(FightingStageBehaviour);

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
	for(auto& iter : _gameManager->characters())
	{
		//iter->stateComponent()->currentFrame()->hitboxes();
	}
	//check collisions
	//resolve game events
}

FightingStageBehaviour::FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j)
	: Behaviour(owner, serviceManager, j)
{
	JSON::TryGetMember(j, "stageBounds", _stageBounds);
	_gameManager = serviceManager->FightingGameManager();
}