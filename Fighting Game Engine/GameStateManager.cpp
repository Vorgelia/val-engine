#include "GameStateManager.h"
#include "FGIncludes.hpp"
#include "Resource.h"
#include "Rendering.h"
namespace GameStateManager
{
	static int stateToLoad = -1;
	int currentState = -1;
	bool isLoading = false;
	std::vector<GameState*> states;
}

void GameStateManager::LoadState(int stateIndex)
{
	DebugLog::Push("----\n\n\n Loading State: " + std::to_string(stateIndex) + "\n\n\n----", LogItem::Type::Message);
	stateToLoad = stateIndex;
}

//The game states are managed here. This is where state loading and cleanup, as well as their per-state callbacks are handled.
void GameStateManager::FrameEnd()
{
	if(stateToLoad > -1)
	{
		if(currentState > -1)
		{
			states[currentState]->Cleanup();
			Resource::Unload();
		}
		currentState = stateToLoad;
		isLoading = true;
		stateToLoad = -1;
		states[currentState]->LoadResources();
	}
}

void GameStateManager::Update()
{
	if(currentState > -1 && isLoading&&states[currentState]->loaded())
	{
		isLoading = false;
		Time::timeSinceLoad = 0;
		Time::lastUpdateTime = Time::time - VE_FRAME_TIME;
		states[currentState]->OnLoaded();
	}
}

void GameStateManager::Init()
{
	//Instantiate all the game states
	//I kind of want to move this somewhere less obscure to make it easier to configure.
	states.push_back(new GS_Intro("States/Intro"));
	states.push_back(new GS_Menu("States/Menu"));
	currentState = -1;
	stateToLoad = 0;
	isLoading = true;
}

void GameStateManager::Cleanup()
{
	Resource::Unload();
}

void GameStateManager::StateInit()
{
	if(currentState < 0 || isLoading)
		return;
	if(!states[currentState]->initialized())
		states[currentState]->Init();
}

void GameStateManager::StateUpdate()
{
	if(currentState < 0 || isLoading)
		return;
	if(states[currentState]->initialized())
		states[currentState]->Update();
}

void GameStateManager::StateGameUpdate()
{
	if(currentState < 0 || isLoading)
		return;
	if(states[currentState]->initialized())
		states[currentState]->GameUpdate();
}

void GameStateManager::StateRenderObjects()
{
	if(currentState < 0 || isLoading)
		return;
	if(states[currentState]->initialized())
		states[currentState]->RenderObjects();
}