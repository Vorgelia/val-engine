#include "FightingGameManager.h"
#include "GameSceneManager.h"
#include "GameCharacter.h"
#include "GameInstance.h"
#include "PlayerManager.h"
#include "GameScene.h"
#include "GLIncludes.hpp"
#include "FightingStageBehaviour.h"
#include "ScreenManager.h"

VE_OBJECT_DEFINITION(FightingGameManager);

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
		_playerManager->ClearPlayers();
		_characters.clear();
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
		ObjectReference<GameCharacter> char1Character = AddCharacter("Characters/Fritz/Prefab.json");
		char1Character->SetOwner(_playerManager->AddPlayer(0, -1));
		break;
	}
	default:
		break;
	}
	_currentState = state;
	FightingGameStateChanged(state);
}

void FightingGameManager::OnInit()
{
	_playerManager = _owningInstance->PlayerManager();
	_gameSceneManager = _owningInstance->GameSceneManager();
	VE_REGISTER_DELEGATE_FUNC(_gameSceneManager->SceneLoaded, HandleSceneLoaded);

	ChangeState(FightingGameState::None);
}
void FightingGameManager::OnDestroyed()
{
	ChangeState(FightingGameState::None);
}

void FightingGameManager::Update()
{
	//PLACEHOLDER
	if(_currentState == FightingGameState::InGame && glfwGetKey(_owningInstance->ScreenManager()->window(), GLFW_KEY_B) == GLFW_PRESS)
	{
		ChangeState(FightingGameState::None);
		_gameSceneManager->LoadScene("Scenes/Intro");
	}
}

ObjectReference<GameCharacter> FightingGameManager::AddCharacter(const std::string& path)
{
	ObjectReference<GameCharacter> character = _gameSceneManager->currentScene()->LoadObject(path);

	if(!character.IsValid())
	{
		return ObjectReference<GameCharacter>();
	}

	_characters.emplace(character);
	return character;
}