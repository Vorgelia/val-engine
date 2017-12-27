#include "ScriptBehaviour.h"
#include "BehaviourFactory.h"
#include "ScriptManager.h"
#include "ServiceManager.h"
#include "Script.h"


VE_BEHAVIOUR_REGISTER_TYPE(ScriptBehaviour);

inline void ScriptBehaviour::RunScriptFunction(std::string name)
{
	if(_script != nullptr && _script->HasFunction(name))
	{
		_script->ExecuteFunction(name, std::vector<std::shared_ptr<BaseScriptVariable>>());
	}
}

void ScriptBehaviour::Init()
{
	RunScriptFunction("Init");
}

void ScriptBehaviour::Update()
{
	RunScriptFunction("Update");
}

void ScriptBehaviour::GameUpdate()
{
	RunScriptFunction("GameUpdate");
}

void ScriptBehaviour::LateUpdate()
{
	RunScriptFunction("LateUpdate");
}

void ScriptBehaviour::OnRenderObjects()
{
	RunScriptFunction("OnRenderObjects");
}

void ScriptBehaviour::OnApplyPostEffects()
{
	RunScriptFunction("OnApplyPostEffects");
}

void ScriptBehaviour::OnRenderUI()
{
	RunScriptFunction("OnRenderUI");
}

void ScriptBehaviour::Cleanup()
{
	RunScriptFunction("Cleanup");

	if(_script != nullptr)
	{
		_scriptManager->RemoveScript(_script);
	}
}

ScriptBehaviour::ScriptBehaviour(Object* owner, ServiceManager* serviceManager, const json& j) : Behaviour(owner, serviceManager, j)
{
	_scriptManager = serviceManager->ScriptManager();
	std::string path;
	if(JSON::TryGetMember<std::string>(j, "script", path))
	{
		_script = _scriptManager->AddScript(path);
	}
}


ScriptBehaviour::~ScriptBehaviour()
{
}
