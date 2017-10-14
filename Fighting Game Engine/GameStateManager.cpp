#include "GameStateManager.h"
#include "FGIncludes.hpp"
#include "Resource.h"
#include "Rendering.h"
#include <unordered_map>

namespace GameStateManager
{
	std::string _stateToLoad = "";
	bool _isLoading = false;
	std::unordered_map<std::string, std::shared_ptr<GameState> const> states;
	std::shared_ptr<GameState> _currentState = nullptr;
}

std::shared_ptr<GameState>& GameStateManager::currentState()
{
	return _currentState;
}

bool GameStateManager::isLoading()
{
	return _isLoading;
}

void GameStateManager::LoadState(const std::string& name)
{
	DebugLog::Push("----\n\n\n Loading State: " + name + "\n\n\n----", LogItem::Type::Message);
	_stateToLoad = name;
}

//The game states are managed here. This is where state loading and cleanup, as well as their per-state callbacks are handled.
void GameStateManager::FrameEnd()
{
	if(!_stateToLoad.empty())
	{
		auto& iter = states.find(_stateToLoad);
		if(iter == states.end())
		{
			DebugLog::Push("GameStateManager - Attempting to load invalid state " + _stateToLoad);
			_stateToLoad.clear();
		}

		if(_currentState != nullptr)
		{
			_currentState->Cleanup();
			Resource::Unload();
		}

		_currentState = iter->second;
		_isLoading = true;
		_stateToLoad.clear();
		_currentState->LoadResources();
	}
}

void GameStateManager::Update()
{
	if(_currentState != nullptr && _isLoading&&_currentState->loaded())
	{
		_isLoading = false;
		Time::OnStateLoaded();
		_currentState->OnLoaded();
	}
}

void GameStateManager::Init()
{
	//Instantiate all the game states
	//TODO: Make dynamic

	states.insert(std::pair<std::string, std::shared_ptr<GameState>>("Intro", std::make_shared<GS_Intro>("States/Intro")));
	states.insert(std::pair<std::string,std::shared_ptr<GameState>>("Menu", std::make_shared<GS_Menu>("States/Menu")));

	_currentState = nullptr;
	_stateToLoad = "Intro";
	_isLoading = true;
}

void GameStateManager::Cleanup()
{
	Resource::Unload();
}

void GameStateManager::StateInit()
{
	if(_currentState == nullptr || _isLoading)
		return;
	if(!_currentState->initialized())
		_currentState->Init();
}

void GameStateManager::StateUpdate()
{
	if(_currentState == nullptr || _isLoading)
		return;
	if(_currentState->initialized())
		_currentState->Update();
}

void GameStateManager::StateGameUpdate()
{
	if(_currentState == nullptr || _isLoading)
		return;
	if(_currentState->initialized())
		_currentState->GameUpdate();
}

void GameStateManager::StateRenderObjects()
{
	if(_currentState == nullptr || _isLoading)
		return;
	if(_currentState->initialized())
		_currentState->RenderObjects();
}