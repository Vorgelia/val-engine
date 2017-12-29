#include "FightingGameManager.h"
#include "GameSceneManager.h"
#include "ServiceManager.h"
#include "Object.h"
#include "PlayerManager.h"
#include "GameScene.h"
#include "GamePlayer.h"
#include "GameCharacter.h"
#include "ServiceManager.h"
#include "Screen.h"

#include "GLIncludes.hpp"


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
		_playerManager->ClearPlayers();
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
	//PLACEHOLDER
	if(_currentState == FightingGameState::InGame && glfwGetKey(_serviceManager->Screen()->window, GLFW_KEY_B) == GLFW_PRESS)
	{
		ChangeState(FightingGameState::None);
		_gameSceneManager->LoadScene("Intro");
	}
}

FightingGameManager::FightingGameManager(ServiceManager* serviceManager) : BaseService(serviceManager)
{
	_allowServiceUpdate = true;
}

FightingGameManager::~FightingGameManager()
{
	ChangeState(FightingGameState::None);
}
