#pragma once
#include <string>
#include "Object.h"
#include "JSON.h"

#define VE_BEHAVIOUR_NAME(nameStr)\
	constexpr std::string name() const override{ return std::string(#nameStr); }

class GameInstance;

class Behaviour
{
protected:
	Object* _owner;
	GameInstance* _serviceManager;

private:
	bool _initialized;

public:
	bool enabled;

	Object* object() const;

	virtual const std::string name() const = 0;

	void TryInit();
	virtual void Init();
	virtual void OnSceneInit();
	virtual void EngineUpdate();
	virtual void GameUpdate();
	virtual void LateGameUpdate();
	virtual void LateEngineUpdate();
	virtual void OnRenderObjects();
	virtual void OnApplyPostEffects();
	virtual void OnRenderUI();
	virtual void Cleanup();

	Behaviour(Object* owner, GameInstance* serviceManager);
	Behaviour(Object* owner, GameInstance* serviceManager, const json& j);
	virtual ~Behaviour();
};