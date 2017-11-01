#include "GameSceneManager.h"
#include "FGIncludes.hpp"
#include "Resource.h"
#include "Rendering.h"
#include <unordered_map>

#define VE_SCENE_FUNCTION_CALL(name, ...)\
	ApplyFunctionToCurrentScene([](GameScene* scene) { scene->name(); }, ##__VA_ARGS__);

namespace GameSceneManager
{
	std::string _sceneToLoad = "";
	bool _isLoading = false;
	std::unordered_map<std::string, std::unique_ptr<GameScene> const> scenes;
	GameScene* _currentState = nullptr;


	void HandleSceneInit();
	bool HandleSceneUpdate();
	void HandleSceneLoad();
	void ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool requiredInitializedState = true);
}

GameScene* GameSceneManager::currentScene()
{
	return _currentState;
}

bool GameSceneManager::isLoading()
{
	return _isLoading;
}

void GameSceneManager::LoadScene(const std::string& name)
{
	DebugLog::Push("----\n\n\n Loading State: " + name + "\n\n\n----", LogItem::Type::Message);
	_sceneToLoad = name;
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
	if(_isLoading && _currentState != nullptr && _currentState->loaded())
	{
		_isLoading = false;
		Time::OnStateLoaded();
		VE_SCENE_FUNCTION_CALL(Init, false);
	}
}

bool GameSceneManager::HandleSceneUpdate()
{
	bool gameUpdated = false;//This is a variable that keeps track of whether we've run a game update on this iteration. If we have, this will tell the engine to render at the end.
	if(!_isLoading)
	{
		VE_SCENE_FUNCTION_CALL(Update);//Send a game loop update regardless of game updates

		//Run game updates until running one would put us ahead of our current time
		while(Time::lastUpdateTime + VE_FRAME_TIME <= Time::time)
		{
			gameUpdated = true;

			Time::FrameUpdate();
			InputManager::Update();
			ScriptManager::Update();

			VE_SCENE_FUNCTION_CALL(GameUpdate);
		}

		VE_SCENE_FUNCTION_CALL(LateUpdate);//Send a late game loop update regardless of game updates
	}

	return gameUpdated;
}

void GameSceneManager::HandleSceneLoad()
{
	if(!_sceneToLoad.empty())
	{
		auto& iter = scenes.find(_sceneToLoad);
		if(iter == scenes.end())
		{
			DebugLog::Push("GameSceneManager - Attempting to load invalid state " + _sceneToLoad);
			_sceneToLoad.clear();
		}

		if(_currentState != nullptr)
		{
			_currentState->Cleanup();
			Resource::Unload();
		}

		_currentState = iter->second.get();
		_isLoading = true;
		_sceneToLoad.clear();
		_currentState->LoadResources();
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
	//TODO: Make dynamic
	scenes.insert(std::make_pair("Intro", std::make_unique<GS_Intro>("States/Intro")));
	scenes.insert(std::make_pair("Menu", std::make_unique<GS_Menu>("States/Menu")));

	_currentState = nullptr;
	_sceneToLoad = "Intro";
	_isLoading = true;
}

void GameSceneManager::Cleanup()
{
	Resource::Unload();
}

void GameSceneManager::ApplyFunctionToCurrentScene(std::function<void(GameScene*)> func, bool requiredInitializedState)
{
	if(_isLoading || _currentState == nullptr || (requiredInitializedState != _currentState->initialized()))
		return;

	func(_currentState);
}