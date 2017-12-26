#include "ServiceManager.h"
#include "DebugLog.h"
#include "GameSceneManager.h"
#include "GLStateTrack.h"
#include "Resource.h"
#include "Rendering.h"
#include "ScriptManager.h"
#include "Time.h"
#include "Screen.h"
#include "InputManager.h"
#include "ResourceLoader.h"

#define ve_named_service_getter(name, type)\
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

#define ve_service_getter(type) ve_named_service_getter(type, type)

ve_service_getter(Debug);
ve_service_getter(GameSceneManager);
ve_service_getter(ResourceManager);
ve_service_getter(ScriptManager);
ve_service_getter(Time);
ve_service_getter(Screen);

ve_named_service_getter(Input, InputManager);
ve_named_service_getter(Filesystem, FilesystemManager);
ve_named_service_getter(Graphics, GraphicsGL);
ve_named_service_getter(Rendering, RenderingGL);

void ServiceManager::InitializeServices()
{
	if(_activeServices.size() > 0)
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

	for(auto& iter : _activeServices)
	{
		(*iter)->Init();
	}
}

void ServiceManager::CleanupServices()
{
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
{
}
