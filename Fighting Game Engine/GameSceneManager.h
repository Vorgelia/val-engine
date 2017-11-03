#pragma once
#include <memory>
#include <vector>

class GameScene;
namespace GameSceneManager
{
	GameScene* currentScene();
	bool isLoading();

	void Init();
	void Update();
	void LoadScene(const std::string& name);
	void RenderScene();
	void Cleanup();
}