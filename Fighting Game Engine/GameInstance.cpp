#include "GameInstance.h"
#include "DebugLog.h"
#include "GameSceneManager.h"
#include "PlayerManager.h"
#include "GraphicsGL.h"
#include "ResourceManager.h"
#include "RenderingGL.h"
#include "ScriptManager.h"
#include "Time.h"
#include "Screen.h"
#include "InputManager.h"
#include "FightingGameManager.h"
#include "FilesystemManager.h"
#include "ObjectFactory.h"

#define VE_NAMED_SERVICE_GETTER(name, type)\
	::##type* GameInstance::name()\
	{\
		if(_##type == nullptr)\
		{\
			_##type = ObjectFactory::CreateObject<::##type>(this);\
			_activeServices.emplace_back(_##type.get());\
		}\
		return _##type.get();\
	}

#define VE_SERVICE_GETTER(type) VE_NAMED_SERVICE_GETTER(type, type)

VE_SERVICE_GETTER(Debug);
VE_SERVICE_GETTER(GameSceneManager);
VE_SERVICE_GETTER(ResourceManager);
VE_SERVICE_GETTER(ScriptManager);
VE_SERVICE_GETTER(ScreenManager);
VE_SERVICE_GETTER(PlayerManager);
VE_SERVICE_GETTER(FightingGameManager);

VE_NAMED_SERVICE_GETTER(Input, InputManager);
VE_NAMED_SERVICE_GETTER(Filesystem, FilesystemManager);
VE_NAMED_SERVICE_GETTER(Graphics, GraphicsGL);
VE_NAMED_SERVICE_GETTER(Rendering, RenderingGL);

VE_OBJECT_DEFINITION(GameInstance);

void GameInstance::OnInit()
{
	_rawConfigData = Filesystem()->LoadFileResource<json>("EngineConfig.json");
	_configData.Deserialize(_rawConfigData);

	//Utilities
	Debug();

	//Rendering API
	ScreenManager();
	Rendering();
	Graphics();

	//File Management
	ResourceManager();
	ScriptManager();

	//Game Management
	Input();
	GameSceneManager();
	PlayerManager();
	FightingGameManager();

	_timeTracker.Reset(glfwGetTime());
}

void GameInstance::OnDestroyed()
{
}

void GameInstance::UpdateServices()
{
	_timeTracker.Update(glfwGetTime());
	_updateDispatcher.DispatchUpdates();
}

GameInstance::GameInstance()
	: _updateDispatcher(this)
{
}