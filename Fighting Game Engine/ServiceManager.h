#pragma once
#include <memory>
#include <vector>
#include "BaseService.h"

#define ve_named_service(name, type) \
	private:\
	std::unique_ptr<BaseService> _s##type;\
	public:\
	::##type* name();

#define ve_service(type) ve_named_service(type, type)

class Debug;
class GameSceneManager;
class GraphicsGL;
class RenderingGL;
class ResourceManager;
class ScriptManager;
class Time;
class Screen;
class InputManager;

class ServiceManager
{
private:
	ve_service(Debug);
	ve_service(GameSceneManager);
	ve_service(ResourceManager);
	ve_service(ScriptManager);
	ve_service(Time);
	ve_service(Screen);

	ve_named_service(Input, InputManager);
	ve_named_service(Graphics, GraphicsGL);
	ve_named_service(Rendering, RenderingGL);

	bool _servicesUpdated;
	std::vector<std::unique_ptr<BaseService>*> _activeServices;

public:
	void InitializeServices();
	void CleanupServices();
	void UpdateServices();

	ServiceManager& operator=(ServiceManager& sm) = delete;
	ServiceManager& operator=(ServiceManager&& sm) = delete;

	ServiceManager(ServiceManager& sm) = delete;
	ServiceManager(ServiceManager&& sm) = delete;

	ServiceManager();
	~ServiceManager();
};

#undef ve_named_service
#undef ve_service