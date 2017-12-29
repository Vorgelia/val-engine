#include "FightingGameManager.h"
#include "GameSceneManager.h"
#include "ServiceManager.h"
#include "Object.h"
#include "PlayerManager.h"
#include "GameScene.h"
#include "GamePlayer.h"
#include "GameCharacter.h"


void FightingGameManager::HandleSceneLoaded(const GameScene* scene)
{
	switch(_currentState)
	{
	case FightingGameState::None:
		ChangeState(FightingGameState::InGame);
		break;
	case FightingGameState::InputDeviceSelection:
		break;
	case FightingGameState::CharacterSelection:
		break;
	case FightingGameState::StageSelection:
		break;
	case FightingGameState::InGame:
		break;
	default:
		break;
	}
}

void FightingGameManager::ChangeState(FightingGameState state)
{
	switch(state)
	{
	case FightingGameState::None:
		break;
	case FightingGameState::InputDeviceSelection:
		break;
	case FightingGameState::CharacterSelection:
		break;
	case FightingGameState::StageSelection:
		break;
	case FightingGameState::InGame:
	{
		Object* char1 = _gameSceneManager->currentScene()->FindObject("Char_1");
		if(char1 == nullptr)
		{
			return;
		}
		GameCharacter* char1CharacterBehaviour = char1->GetBehaviour<GameCharacter>("GameCharacter");
		char1CharacterBehaviour->SetOwner(_playerManager->AddPlayer(0, -1));
		break;
	}
	default:
		break;
	}
	_currentState = state;
}

void FightingGameManager::Init()
{
	_playerManager = _serviceManager->PlayerManager();

	_gameSceneManager = _serviceManager->GameSceneManager();
	_gameSceneManager->SceneLoaded += GameSceneManager::GameSceneEventHandler::func_t([this](const GameScene* scene) { HandleSceneLoaded(scene); });

	_currentState = FightingGameState::None;
}

void FightingGameManager::Update()
{

}

FightingGameManager::FightingGameManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{
}


FightingGameManager::~FightingGameManager()
{
}
