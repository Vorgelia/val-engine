#include "GameStateManager.h"
#include "FGIncludes.hpp"
#include "Resource.h"
#include "Rendering.h"
namespace GameStateManager{
	static int _stateToLoad=-1;
	int currentState = -1;
	bool isLoading=false;
	std::vector<GameState*> states;
}

void GameStateManager::LoadState(int stl){
	std::cout << "----\n\n\nLS: " << stl << "\n\n\n----"<<std::endl;
	_stateToLoad = stl;
}

void GameStateManager::FrameEnd(){
	if (_stateToLoad > -1){
		if (currentState > -1){
			states[currentState]->Cleanup();
			Resource::Unload();
		}
		currentState = _stateToLoad;
		isLoading = true;
		_stateToLoad = -1;
		states[currentState]->LoadResources();
	}		
}

void GameStateManager::Update(){
	if (currentState>-1&&isLoading&&states[currentState]->loaded()){
		isLoading = false;
		Time::timeSinceLoad = 0;
		states[currentState]->OnLoaded();
	}
}

void GameStateManager::Init(){
	//Instantiate all the game states
	states.push_back(new GS_Intro("States/Intro"));
	states.push_back(new GS_Menu("States/Menu"));
	currentState = -1;
	_stateToLoad = 0;
	isLoading = true;
}

void GameStateManager::Cleanup(){
	Resource::Unload();
}