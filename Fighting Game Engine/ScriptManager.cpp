#include "ScriptManager.h"
#include "Script.h"
#include "BaseScriptVariable.h"
#include "ResourceLoader.h"
#include <unordered_map>

#include "DebugLog.h"

namespace ScriptManager
{
	std::unordered_map<std::string, std::shared_ptr<Script>> _scripts;
	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _globalVariables;

	void HandleScriptBindings(const std::shared_ptr<Script> script);
}

void ScriptManager::Init()
{
	AddScript("Scripts/Base/example.vscript");
}

void ScriptManager::AddScript(const FS::path& path)
{
	std::vector<std::string> lines = ResourceLoader::ReturnFileLines(path, false);
	if(lines.size() > 0)
	{
		const std::string& scriptName = path.leaf().generic_string();
		std::shared_ptr<Script> script = std::make_shared<Script>(scriptName, lines);
		_scripts.emplace(scriptName, script);

		HandleScriptBindings(script);
		script->Init();
	}
}

void ScriptManager::HandleScriptBindings(const std::shared_ptr<Script> script)
{
	std::vector<std::string> &bindings = script->GetPragmaDirectives("Bind");

	for(const auto& directive : bindings)
	{
		if(directive == "UI")
		{
			//handle UI bindings
		}
		else if(directive == "Debug")
		{
			script->BindFunction("ve_log", &DebugLog::Push);
		}
	}
}

void ScriptManager::AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable)
{
	_globalVariables.emplace(name, variable);
}

std::shared_ptr<BaseScriptVariable> ScriptManager::GetVariable(const std::string& name)
{
	auto& iter = _globalVariables.find(name);
	if(iter == _globalVariables.end())
	{
		return nullptr;
	}
	return iter->second;
}

void ScriptManager::Update()
{
	for(auto& script : _scripts)
	{
		if(script.second->valid())
		{
			script.second->Execute();
		}
	}
}

void ScriptManager::Cleanup()
{
	_scripts.clear();
}