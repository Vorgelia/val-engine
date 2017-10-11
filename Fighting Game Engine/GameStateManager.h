#pragma once
#include <vector>

class GameState;
namespace GameStateManager
{
	//global state variables here
	extern std::vector<GameState*> states;

	extern int stateToLoad;
	extern int currentState;
	extern bool isLoading;

	void Init();
	void Update();
	void LoadState(int stateIndex);
	void FrameEnd();
	void Cleanup();

	void StateInit();
	void StateUpdate();
	void StateGameUpdate();
	void StateRenderObjects();
}