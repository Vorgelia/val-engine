#include "BaseGameCharacterComponent.h"
#include "GameCharacter.h"
#include "GameScene.h"
#include "FightingStageBehaviour.h"
#include "FightingGameManager.h"

void BaseGameCharacterComponent::OnInit()
{
	_gameCharacterOwner = dynamic_cast<GameCharacter*>(_owner.get());
	_fightingSceneBehavior = dynamic_cast<FightingStageBehaviour*>(owningScene()->sceneBehavior());
	_fightingGameManager = dynamic_cast<FightingGameManager*>(_owningInstance->gameManager());
}
