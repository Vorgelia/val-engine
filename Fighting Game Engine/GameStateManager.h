#pragma once
#include "CommonUtilIncludes.hpp"
class GameState;
namespace GameStateManager{
	//global state variables here
	extern std::vector<GameState*> states;

	extern int _stateToLoad;
	extern int currentState;
	extern bool isLoading;

	void Init();
	void Update();
	void LoadState(int stl);
	void FrameEnd();
	void Cleanup();
}