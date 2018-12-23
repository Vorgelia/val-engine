#include "GameSceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GraphicsGL.h"
#include "RenderingGL.h"
#include "GameScene.h"
#include "DebugLog.h"
#include "GameInstance.h"
#include "Time.h"

#define VE_SCENE_FUNCTION_CALL(fnName, ...)\
	ApplyFunctionToCurrentScene([](GameScene* scene) { scene->fnName(); }, ##__VA_ARGS__);

GameScene* GameSceneManager::currentScene() const
{
	return _currentScene.get();
}

bool GameSceneManager::isLoading() const
{
	return _isLoading;
}

void GameSceneManager::OnInit()
{
	_debug = _owningInstance->Debug();

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::FrameEnd, UpdateType::LastFixedGameUpdate, UpdateService);

	LoadScene(_owningInstance->configData().gameConfigData.defaultScene);
}

void GameSceneManager::OnDestroyed()
{
	
}

void GameSceneManager::UpdateService()
{
	if(!_sceneToLoad.empty())
	{
		_debug->VE_LOG("----\n\n\n Loading Scene: " + _sceneToLoad + "\n\n\n----", LogItem::Type::Message);

		if(_currentScene != nullptr)
		{
			_currentScene.reset();
		}

		_isLoading = true;

		_currentScene = ObjectFactory::CreateObjectDeferred<GameScene>();
		_currentScene->_dataPath = "/Scenes/"+_sceneToLoad+".json";
		ObjectFactory::InitializeObject(_currentScene.get(), this);
		_sceneToLoad.clear();

		SceneLoaded(_currentScene.get());
		_isLoading = false;
	}
}

void GameSceneManager::LoadScene(const std::string& name)
{
	_sceneToLoad = name;
}

GameSceneManager::GameSceneManager()
	: _debug(nullptr)
	, _currentScene(nullptr)
{
	
}
