#include "GameSceneManager.h"
#include "FGIncludes.hpp"
#include "Resource.h"
#include "Rendering.h"
#include <unordered_map>

namespace GameSceneManager
{
	std::string _stateToLoad = "";
	bool _isLoading = false;
	std::unordered_map<std::string, std::unique_ptr<GameScene> const> states;
	GameScene* _currentState = nullptr;

	void ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool ignoreInitializedCheck = false);
}

GameScene* GameSceneManager::currentState()
{
	return _currentState;
}

bool GameSceneManager::isLoading()
{
	return _isLoading;
}

void GameSceneManager::LoadState(const std::string& name)
{
	DebugLog::Push("----\n\n\n Loading State: " + name + "\n\n\n----", LogItem::Type::Message);
	_stateToLoad = name;
}

//The game states are managed here. This is where state loading and cleanup, as well as their per-state callbacks are handled.
void GameSceneManager::FrameEnd()
{
	if(!_stateToLoad.empty())
	{
		auto& iter = states.find(_stateToLoad);
		if(iter == states.end())
		{
			DebugLog::Push("GameSceneManager - Attempting to load invalid state " + _stateToLoad);
			_stateToLoad.clear();
		}

		if(_currentState != nullptr)
		{
			_currentState->Cleanup();
			Resource::Unload();
		}

		_currentState = iter->second.get();
		_isLoading = true;
		_stateToLoad.clear();
		_currentState->LoadResources();
	}
}

void GameSceneManager::Update()
{
	if(_isLoading && _currentState != nullptr && _currentState->loaded())
	{
		_isLoading = false;
		Time::OnStateLoaded();
		_currentState->OnLoaded();
	}
}

void GameSceneManager::Init()
{
	//Instantiate all the game states
	//TODO: Make dynamic
	states.insert(std::make_pair("Intro", std::make_unique<GS_Intro>("States/Intro")));
	states.insert(std::make_pair("Menu", std::make_unique<GS_Menu>("States/Menu")));

	_currentState = nullptr;
	_stateToLoad = "Intro";
	_isLoading = true;
}

void GameSceneManager::Cleanup()
{
	Resource::Unload();
}

void GameSceneManager::ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool ignoreInitializedCheck)
{
	if(_isLoading || _currentState == nullptr || (!ignoreInitializedCheck && !_currentState->initialized()))
		return;

	func(_currentState);
}

void GameSceneManager::StateInit()
{
	ApplyFunctionToCurrentScene([](GameScene* scene) { if(!scene->initialized()) scene->Init(); }, true);
}

void GameSceneManager::StateUpdate()
{
	ApplyFunctionToCurrentScene([](GameScene* scene) { scene->Update(); });
}

void GameSceneManager::StateGameUpdate()
{
	ApplyFunctionToCurrentScene([](GameScene* scene) { scene->GameUpdate(); });
}

void GameSceneManager::StateRenderObjects()
{
	ApplyFunctionToCurrentScene([](GameScene* scene) { scene->RenderObjects(); });
}