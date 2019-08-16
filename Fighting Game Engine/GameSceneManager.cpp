#include "GameSceneManager.h"
#include "ResourceManager.h"
#include "InputManager.h"
#include "GraphicsGL.h"
#include "BaseSceneBehavior.h"
#include "RenderingGL.h"
#include "GameScene.h"
#include "DebugLog.h"
#include "GameInstance.h"

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
	_resource = _owningInstance->ResourceManager();

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::FrameEnd, UpdateType::LastFixedGameUpdate, UpdateService);
}

void GameSceneManager::OnServiceInit()
{
	LoadScene(_owningInstance->configData().gameConfigData.defaultScene);
}

void GameSceneManager::OnServiceCleanup()
{
	_currentScene.reset();
}

void GameSceneManager::UpdateService()
{
	if(!_sceneToLoad.empty())
	{
		json* sceneJson = _resource->GetJsonData(_sceneToLoad + ".json");
		_sceneToLoad.clear();
		if(sceneJson == nullptr)
		{
			return;
		}

		_debug->VE_LOG("----\n\n\n Loading Scene: " + _sceneToLoad + "\n\n\n----", LogItem::Type::Message);

		_isLoading = true;

		JsonSerializationProxy proxy{ *sceneJson };
		_currentScene = ObjectFactory::CreateObjectFromDataDeferred<GameScene>(this, proxy);
		ObjectFactory::InitializeObject(_currentScene.get(), this, &proxy);
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
