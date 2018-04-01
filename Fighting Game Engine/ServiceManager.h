#pragma once
#include <memory>
#include <vector>
#include "BaseService.h"

#define VE_NAMED_SERVICE(name, type) \
	private:\
	std::unique_ptr<BaseService> _s##type;\
	public:\
	::##type* name();

#define VE_SERVICE(type) VE_NAMED_SERVICE(type, type)

class Debug;
class GameSceneManager;
class GraphicsGL;
class RenderingGL;
class ResourceManager;
class ScriptManager;
class Time;
class Screen;
class InputManager;
class FilesystemManager;
class PlayerManager;
class FightingGameManager;

class ServiceManager
{
private:
	VE_SERVICE(Debug);
	VE_SERVICE(GameSceneManager);
	VE_SERVICE(ResourceManager);
	VE_SERVICE(ScriptManager);
	VE_SERVICE(Time);
	VE_SERVICE(Screen);
	VE_SERVICE(PlayerManager);
	VE_SERVICE(FightingGameManager);

	VE_NAMED_SERVICE(Input, InputManager);
	VE_NAMED_SERVICE(Filesystem, FilesystemManager);
	VE_NAMED_SERVICE(Graphics, GraphicsGL);
	VE_NAMED_SERVICE(Rendering, RenderingGL);

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

#undef VE_NAMED_SERVICE
#undef VE_SERVICE