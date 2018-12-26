#pragma once
#include "BaseService.h"
#include "Delegate.h"
#include <memory>
#include <functional>
#include <filesystem>

class GameScene;
class GameInstance;

class Debug;
class RenderingGL;
class ResourceManager;
class Time;
class InputManager;

class GameSceneManager : public BaseService
{
private:
	Debug* _debug;

	ve::unique_object_ptr<GameScene> _currentScene;

	std::string _sceneToLoad = "";
	bool _isLoading = false;

public:
	typedef Delegate<const GameScene*> GameSceneEventHandler;
	GameSceneEventHandler SceneLoaded;

	GameScene* currentScene() const;
	bool isLoading() const;

	void OnInit() override;
	void OnServiceInit() override;
	void OnDestroyed() override;

	void UpdateService();

	void LoadScene(const std::string& name);

	GameSceneManager();
	virtual ~GameSceneManager() = default;
};