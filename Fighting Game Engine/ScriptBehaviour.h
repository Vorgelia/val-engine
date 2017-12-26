#pragma once
#include "Behaviour.h"

class ScriptBehaviour :
	public Behaviour
{
	friend class ScriptManager;

private:
	ScriptManager* _scriptManager;

private:
	Script* _script;

	void RunScriptFunction(std::string name);
public:
	VE_BEHAVIOUR_NAME(ScriptBehaviour);

	VE_BEHAVIOUR_REGISTER_FUNCTION(Init);
	VE_BEHAVIOUR_REGISTER_FUNCTION(Update);
	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);
	VE_BEHAVIOUR_REGISTER_FUNCTION(LateUpdate);
	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderObjects);
	VE_BEHAVIOUR_REGISTER_FUNCTION(OnApplyPostEffects);
	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderUI);
	VE_BEHAVIOUR_REGISTER_FUNCTION(Cleanup);

	ScriptBehaviour(Object* owner, ServiceManager* serviceManager, const json& j);
	~ScriptBehaviour();
};

