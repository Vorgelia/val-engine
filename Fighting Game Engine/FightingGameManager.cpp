#include "FightingGameManager.h"
#include "GameSceneManager.h"
#include "GameInstance.h"
#include "Object.h"
#include "PlayerManager.h"
#include "GameScene.h"
#include "GameCharacter.h"
#include "Screen.h"
#include "GameCharacter.h"
#include "GLIncludes.hpp"
#include "FightingStageBehaviour.h"


void FightingGameManager::HandleSceneLoaded(const GameScene* scene)
{
	//PLACEHOLDER
	switch(_currentState)
	{
	case FightingGameState::None:
		if(scene->name() == "Menu")
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
	if(state == _currentState)
	{
		return;
	}

	switch(state)
	{
	case FightingGameState::None:
		CleanupCharacters();
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
		//PLACEHOLDER
		GameCharacter* char1CharacterBehaviour = AddCharacter("Characters/Fritz/Prefab.json");
		char1CharacterBehaviour->SetOwner(_playerManager->AddPlayer(0, -1));

		_stageBehaviour = dynamic_cast<FightingStageBehaviour*>(_gameSceneManager->currentScene()->FindBehaviour("FightingStageBehaviour"));
		break;
	}
	default:
		break;
	}
	_currentState = state;
	FightingGameStateChanged(state);
}

void FightingGameManager::Init()
{
	_playerManager = _serviceManager->PlayerManager();
	_gameSceneManager = _serviceManager->GameSceneManager();
	_gameSceneManager->SceneLoaded += VE_DELEGATE_FUNC(GameSceneManager::GameSceneEventHandler, HandleSceneLoaded);

	ChangeState(FightingGameState::None);
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

void FightingGameManager::Cleanup()
{
	ChangeState(FightingGameState::None);
}

GameCharacter* FightingGameManager::AddCharacter(const std::string& path)
{
	Object* object = _gameSceneManager->currentScene()->LoadObject(path);
	if(object == nullptr)
	{
		return nullptr;
	}

	GameCharacter* characterBehaviour = object->GetBehaviour<GameCharacter>("GameCharacter");
	if(characterBehaviour != nullptr)
	{
		_characters.emplace(characterBehaviour);
	}

	return characterBehaviour;
}

void FightingGameManager::RemoveCharacter(int id)
{
	Object* object = _gameSceneManager->currentScene()->FindObject(id);
	if(object == nullptr)
	{
		return;
	}

	GameCharacter* character = object->GetBehaviour<GameCharacter>("GameCharacter");
	_characters.erase(character);
	_gameSceneManager->currentScene()->DestroyObject(object);
}

void FightingGameManager::CleanupCharacters()
{
	for(auto& iter : _characters)
	{
		_gameSceneManager->currentScene()->DestroyObject(iter->object());
	}

	_characters.clear();
}

FightingGameState FightingGameManager::currentState() const
{
	return _currentState;
}

FightingStageBehaviour* FightingGameManager::stageBehaviour() const
{
	return _stageBehaviour;
}

const std::unordered_set<GameCharacter*>& FightingGameManager::characters() const
{
	return _characters;
}

FightingGameManager::FightingGameManager(GameInstance* serviceManager) : BaseService(serviceManager, 500)
{
	_allowServiceUpdate = true;
}
