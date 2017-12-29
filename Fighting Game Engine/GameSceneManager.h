#pragma once
#include "BaseService.h"
#include "Delegate.h"
#include <memory>
#include <vector>
#include <functional>
#include <unordered_map>

class GameScene;
class ServiceManager;

class Debug;
class RenderingGL;
class ResourceManager;
class Time;
class InputManager;

class GameSceneManager : public BaseService
{
private:
	Debug* _debug;
	RenderingGL* _rendering;
	ResourceManager* _resourceManager;
	Time* _time;
	InputManager* _input;

private:

	std::unordered_map<std::string, std::unique_ptr<GameScene> const> _scenes;

	GameScene* _currentScene = nullptr;
	std::string _currentSceneName = "";

	std::string _sceneToLoad = "";
	bool _isLoading = false;

	void HandleSceneInit();
	bool HandleSceneUpdate();
	void HandleSceneLoad();

	void ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool requiredInitializedState = true);

public:
	typedef Delegate<const GameScene*> GameSceneEventHandler;
	GameSceneEventHandler SceneLoaded;

	GameScene* currentScene();
	bool isLoading();

	void Update() override;
	void Init() override;

	void LoadScene(const std::string& name);
	void ReloadScene();
	void RenderScene();

	GameSceneManager(ServiceManager* serviceManager);
	virtual ~GameSceneManager();
};