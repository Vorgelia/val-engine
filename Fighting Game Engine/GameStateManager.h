#pragma once
#include <memory>
#include <vector>

class GameState;
namespace GameStateManager
{
	GameState* currentState();
	bool isLoading();

	void Init();
	void Update();
	void LoadState(const std::string& name);
	void FrameEnd();
	void Cleanup();

	void StateInit();
	void StateUpdate();
	void StateGameUpdate();
	void StateRenderObjects();
}