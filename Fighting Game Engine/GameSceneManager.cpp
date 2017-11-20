#include "GameSceneManager.h"
#include "FGIncludes.hpp"
#include "Resource.h"
#include "Rendering.h"
#include <unordered_map>

#define VE_SCENE_FUNCTION_CALL(fnName, ...)\
	ApplyFunctionToCurrentScene([](GameScene* scene) { scene->fnName(); }, ##__VA_ARGS__);

namespace GameSceneManager
{
	std::string _sceneToLoad = "";
	bool _isLoading = false;
	std::unordered_map<std::string, std::unique_ptr<GameScene> const> scenes;
	GameScene* _currentScene = nullptr;
	std::string _currentSceneName = "";

	void HandleSceneInit();
	bool HandleSceneUpdate();
	void HandleSceneLoad();
	void ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool requiredInitializedState = true);
}

GameScene* GameSceneManager::currentScene()
{
	return _currentScene;
}

bool GameSceneManager::isLoading()
{
	return _isLoading;
}

void GameSceneManager::LoadScene(const std::string& name)
{
	_sceneToLoad = name;
}

void GameSceneManager::ReloadScene()
{
	if(_currentScene != nullptr && !_isLoading)
	{
		LoadScene(_currentSceneName);
	}
}

void GameSceneManager::RenderScene()
{
	Rendering::BeginFrame();

	VE_SCENE_FUNCTION_CALL(RenderObjects);

	VE_SCENE_FUNCTION_CALL(ApplyPostEffects);

	VE_SCENE_FUNCTION_CALL(RenderUI);

	Rendering::EndFrame();
}

void GameSceneManager::HandleSceneInit()
{
	if(_isLoading && _currentScene != nullptr && _currentScene->loaded())
	{
		_isLoading = false;
		Time::OnSceneLoaded();
		VE_SCENE_FUNCTION_CALL(Init, false);
	}
}

bool GameSceneManager::HandleSceneUpdate()
{
	bool gameUpdated = false;//This is a variable that keeps track of whether we've run a game update on this iteration. If we have, this will tell the engine to render at the end.
	if(!_isLoading)
	{
		VE_SCENE_FUNCTION_CALL(Update);//Send a game loop update regardless of game updates

		int updateCount = 20;
		//Run game updates until running one would put us ahead of our current time
		while((Time::lastUpdateTime + VE_FRAME_TIME <= Time::time) && ((--updateCount) >= 0))
		{
			gameUpdated = true;

			Time::FrameUpdate();
			InputManager::Update();
			ScriptManager::Update();

			VE_SCENE_FUNCTION_CALL(GameUpdate);
			VE_SCENE_FUNCTION_CALL(LateGameUpdate);
		}

		VE_SCENE_FUNCTION_CALL(LateUpdate);//Send a late game loop update regardless of game updates
	}

	return gameUpdated;
}

void GameSceneManager::HandleSceneLoad()
{
	if(!_sceneToLoad.empty())
	{
		VE_DEBUG_LOG("----\n\n\n Loading Scene: " + _sceneToLoad + "\n\n\n----", LogItem::Type::Message);

		auto& iter = scenes.find(_sceneToLoad);
		if(iter == scenes.end())
		{
			VE_DEBUG_LOG("GameSceneManager - Attempting to load invalid scene " + _sceneToLoad);
			_sceneToLoad.clear();
		}

		if(_currentScene != nullptr)
		{
			_currentScene->Cleanup();
			Resource::Unload();
		}

		_currentScene = iter->second.get();
		_isLoading = true;
		_currentSceneName = _sceneToLoad;
		_sceneToLoad.clear();
		_currentScene->LoadResources();
	}
}

void GameSceneManager::Update()
{
	HandleSceneInit();
	bool gameUpdated = HandleSceneUpdate();
	HandleSceneLoad();

	if(gameUpdated)
	{
		RenderScene();
	}
}

void GameSceneManager::Init()
{
	//Instantiate all the game scenes
	FS::directory_iterator dir(FS::path("Scenes/"));
	FS::directory_iterator end;

	while(dir != end)
	{
		if(!FS::is_directory(*dir))
		{
			continue;
		}

		scenes.emplace(
			std::make_pair(
				dir->path().leaf().string(),
				std::make_unique<GameScene>(dir->path().string())));
		++dir;
	}

	_currentScene = nullptr;
	_sceneToLoad = "Intro";
	_isLoading = true;
}

void GameSceneManager::Cleanup()
{
	Resource::Unload();
}

void GameSceneManager::ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool requiredInitializedState)
{
	if(_isLoading || _currentScene == nullptr || (requiredInitializedState != _currentScene->initialized()))
		return;

	func(_currentScene);
}