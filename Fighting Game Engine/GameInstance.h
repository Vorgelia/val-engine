#pragma once
#include <memory>
#include <vector>
#include "BaseService.h"
#include "UpdateDispatcher.h"
#include "GameObject.h"
#include "EngineConfigData.h"
#include "TimeTracker.h"

#define VE_NAMED_SERVICE(name, type) \
	private:\
	ve::unique_object_ptr<class type> _##type;\
	public:\
	::##type* name();\

#define VE_SERVICE(type) VE_NAMED_SERVICE(type, type)

class BaseGameManager;

class GameInstance : public BaseObject
{
	VE_OBJECT_DECLARATION(GameInstance, BaseObject);

private:
	VE_SERVICE(Debug);
	VE_SERVICE(GameSceneManager);
	VE_SERVICE(ResourceManager);
	VE_SERVICE(ScriptManager);
	VE_SERVICE(ScreenManager);
	VE_SERVICE(PlayerManager);

	VE_NAMED_SERVICE(Input, InputManager);
	VE_NAMED_SERVICE(Filesystem, FilesystemManager);
	VE_NAMED_SERVICE(Graphics, GraphicsGL);
	VE_NAMED_SERVICE(Rendering, RenderingGL);

private:
	TimeTracker _timeTracker;
	ve::unique_object_ptr<UpdateDispatcher> _updateDispatcher;

	ve::unique_object_ptr<BaseGameManager> _gameManager;

	std::vector<ObjectReference<BaseService>> _activeServices;

	EngineConfigData _configData;
	json _rawConfigData;

public:
	const TimeTracker& timeTracker() const { return _timeTracker; }
	UpdateDispatcher& updateDispatcher() const { return *_updateDispatcher.get(); }
	BaseGameManager* gameManager() const { return _gameManager.get(); }
	const EngineConfigData& configData() const { return _configData; }
	const json& rawConfigData() const { return _rawConfigData; }

	void OnInit() override;
	void OnDestroyed() override;

	void UpdateServices();

	GameInstance& operator=(GameInstance& sm) = delete;
	GameInstance& operator=(GameInstance&& sm) = delete;

	GameInstance(GameInstance& sm) = delete;
	GameInstance(GameInstance&& sm) = delete;

	GameInstance();
	~GameInstance() = default;
};

#undef VE_NAMED_SERVICE
#undef VE_SERVICE