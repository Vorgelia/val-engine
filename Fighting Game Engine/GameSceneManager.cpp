#include "GameSceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GraphicsGL.h"
#include "RenderingGL.h"
#include "GameScene.h"
#include "DebugLog.h"
#include "ServiceManager.h"
#include "Time.h"

#define VE_SCENE_FUNCTION_CALL(fnName, ...)\
	ApplyFunctionToCurrentScene([](GameScene* scene) { scene->fnName(); }, ##__VA_ARGS__);

GameScene* GameSceneManager::currentScene() const
{
	return _currentScene;
}

bool GameSceneManager::isLoading() const
{
	return _isLoading;
}

void GameSceneManager::LoadScene(const std::string& name)
{
	if(!_isLoading)
	{
		_sceneToLoad = name;
	}
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
	_rendering->BeginFrame();

	VE_SCENE_FUNCTION_CALL(RenderObjects);

	VE_SCENE_FUNCTION_CALL(ApplyPostEffects);

	VE_SCENE_FUNCTION_CALL(RenderUI);

	_rendering->EndFrame();
}

void GameSceneManager::HandleSceneInit()
{
	if(_isLoading && _currentScene != nullptr && _currentScene->loaded())
	{
		_isLoading = false;
		_time->HandleSceneLoaded();
		VE_SCENE_FUNCTION_CALL(Init, false);
	}
}

bool GameSceneManager::HandleSceneUpdate()
{
	if(_isLoading)
	{
		return false;
	}

	//This is a variable that keeps track of whether we've run a game update on this iteration. If we have, this will tell the engine to render at the end.
	bool gameUpdated = false;
	VE_SCENE_FUNCTION_CALL(EngineUpdate);//Send an engine update regardless of game updates

	int updateCount = 2;
	//Run game updates until running one would put us ahead of our current time
	while((_time->lastUpdateTime + VE_FRAME_TIME <= _time->time) && ((--updateCount) >= 0))
	{
		gameUpdated = true;

		_time->FrameUpdate();
		_input->FrameUpdate();

		VE_SCENE_FUNCTION_CALL(GameUpdate);
		VE_SCENE_FUNCTION_CALL(LateGameUpdate);
	}

	VE_SCENE_FUNCTION_CALL(LateEngineUpdate);//Send a late engine update regardless of game updates

	return gameUpdated;
}

void GameSceneManager::HandleSceneLoad()
{
	if(!_sceneToLoad.empty())
	{
		_debug->VE_LOG("----\n\n\n Loading Scene: " + _sceneToLoad + "\n\n\n----", LogItem::Type::Message);

		auto& iter = _scenes.find(_sceneToLoad);
		if(iter == _scenes.end())
		{
			_debug->VE_LOG("GameSceneManager - Attempting to load invalid scene " + _sceneToLoad);
			_sceneToLoad.clear();
		}

		if(_currentScene != nullptr)
		{
			_currentScene->Cleanup();
			_resourceManager->Unload();
		}

		_currentScene = iter->second.get();
		_isLoading = true;
		_currentSceneName = _sceneToLoad;
		_sceneToLoad.clear();
		_currentScene->LoadResources();

		SceneLoaded(_currentScene);
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
	_debug = _serviceManager->Debug();
	_rendering = _serviceManager->Rendering();
	_resourceManager = _serviceManager->ResourceManager();
	_time = _serviceManager->Time();
	_input = _serviceManager->Input();
}

void GameSceneManager::Cleanup()
{
	_resourceManager->Unload();
}

void GameSceneManager::ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool requiredInitializedState) const
{
	if(_isLoading || _currentScene == nullptr || (requiredInitializedState != _currentScene->initialized()))
		return;

	func(_currentScene);
}

GameSceneManager::GameSceneManager(ServiceManager* serviceManager) :
	BaseService(serviceManager, 100)
{
	_allowServiceUpdate = true;

	//Instantiate all the game scenes
	FS::directory_iterator dir(FS::path("Scenes/"));
	FS::directory_iterator end;

	while(dir != end)
	{
		if(!FS::is_directory(*dir))
		{
			continue;
		}

		_scenes.emplace(
			std::make_pair(
				dir->path().leaf().string(),
				std::make_unique<GameScene>(dir->path().string(), _serviceManager)));
		++dir;
	}

	_currentScene = nullptr;
	_sceneToLoad = "Intro";
	_isLoading = true;
}

GameSceneManager::~GameSceneManager()
= default;
