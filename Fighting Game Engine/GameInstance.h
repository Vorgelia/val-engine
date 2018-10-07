#pragma once
#include <memory>
#include <vector>
#include "BaseService.h"
#include "UpdateDispatcher.h"
#include "GameObject.h"

#define VE_NAMED_SERVICE(name, type) \
	private:\
	ve::unique_object_ptr<class type> _##type;\
	public:\
	type* name();

#define VE_SERVICE(type) VE_NAMED_SERVICE(type, type)

class GameInstance : public BaseObject
{
private:
	VE_SERVICE(Debug);
	VE_SERVICE(GameSceneManager);
	VE_SERVICE(ResourceManager);
	VE_SERVICE(ScriptManager);
	VE_SERVICE(Screen);
	VE_SERVICE(PlayerManager);
	VE_SERVICE(FightingGameManager);

	VE_NAMED_SERVICE(Input, InputManager);
	VE_NAMED_SERVICE(Filesystem, FilesystemManager);
	VE_NAMED_SERVICE(Graphics, GraphicsGL);
	VE_NAMED_SERVICE(Rendering, RenderingGL);

private:
	ObjectFactory _objectFactory;
	TimeTracker _timeTracker;
	UpdateDispatcher _updateDispatcher;

	std::vector<ObjectReference<BaseService>> _activeServices;

	json _configData;

public:
	void OnInit() override;
	void OnDestroyed() override;

	void UpdateServices();

	const ObjectFactory& objectFactory() const { return _objectFactory; }
	const TimeTracker& timeTracker() const { return _timeTracker; }
	const UpdateDispatcher& updateDispatcher() const { return _updateDispatcher; }

	const json& configData() const { return _configData; }

	GameInstance& operator=(GameInstance& sm) = delete;
	GameInstance& operator=(GameInstance&& sm) = delete;

	GameInstance(GameInstance& sm) = delete;
	GameInstance(GameInstance&& sm) = delete;

	GameInstance();
	~GameInstance() = default;
};

#undef VE_NAMED_SERVICE
#undef VE_SERVICE