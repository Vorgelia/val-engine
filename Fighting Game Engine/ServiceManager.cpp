#include "ServiceManager.h"
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

#define VE_NAMED_SERVICE_GETTER(name, type)\
	::##type* ServiceManager::name()\
	{\
		if(_s##type == nullptr)\
		{\
			_s##type = std::make_unique<::##type>(this);\
			_activeServices.push_back(&_s##type);\
			_servicesUpdated = true;\
		}\
		return static_cast<::##type*>(_s##type.get());\
	}

#define VE_SERVICE_GETTER(type) VE_NAMED_SERVICE_GETTER(type, type)

VE_SERVICE_GETTER(Debug);
VE_SERVICE_GETTER(GameSceneManager);
VE_SERVICE_GETTER(ResourceManager);
VE_SERVICE_GETTER(ScriptManager);
VE_SERVICE_GETTER(Time);
VE_SERVICE_GETTER(Screen);
VE_SERVICE_GETTER(PlayerManager);
VE_SERVICE_GETTER(FightingGameManager);

VE_NAMED_SERVICE_GETTER(Input, InputManager);
VE_NAMED_SERVICE_GETTER(Filesystem, FilesystemManager);
VE_NAMED_SERVICE_GETTER(Graphics, GraphicsGL);
VE_NAMED_SERVICE_GETTER(Rendering, RenderingGL);

void ServiceManager::InitializeServices()
{
	if(!_activeServices.empty())
	{
		CleanupServices();
	}

	//Utilities
	Time();
	Debug();
	Input();

	//Rendering API
	Screen();
	Rendering();
	Graphics();

	//File Management
	Filesystem();
	ResourceManager();
	ScriptManager();

	//Game Management
	GameSceneManager();
	PlayerManager();
	FightingGameManager();

	for(auto& iter : _activeServices)
	{
		(*iter)->Init();
	}
}

void ServiceManager::CleanupServices()
{
	for(auto& iter : _activeServices)
	{
		(*iter)->Cleanup();
	}

	for(std::unique_ptr<BaseService>* service : _activeServices)
	{
		BaseService* ptr = service->release();
		delete ptr;
	}
}

void ServiceManager::UpdateServices()
{
	if(_servicesUpdated)
	{
		std::sort(_activeServices.begin(), _activeServices.end(), [](std::unique_ptr<BaseService>* a, std::unique_ptr<BaseService>* b) -> bool
		{
			return (*a)->serviceUpdateSortingOrder() > (*b)->serviceUpdateSortingOrder();
		});
		_servicesUpdated = false;
	}

	for(std::unique_ptr<BaseService>* service : _activeServices)
	{
		if((*service)->allowServiceUpdate())
		{
			(*service)->Update();
		}
	}
}

ServiceManager::ServiceManager()
{
	_servicesUpdated = true;
}

ServiceManager::~ServiceManager()
= default;
