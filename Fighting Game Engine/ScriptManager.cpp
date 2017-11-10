#include "ScriptManager.h"
#include "Script.h"
#include "BaseScriptVariable.h"
#include "ScriptVariable.h"
#include "ScriptCollection.h"
#include "ResourceLoader.h"
#include <unordered_map>
#include <memory>
#include "ScriptBehaviour.h"
#include "DebugLog.h"
#include "Time.h"

std::unordered_map<std::string, std::unique_ptr<Script>> ScriptManager::_scripts;
std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> ScriptManager::_globalVariables;

void ScriptManager::Init()
{
	std::shared_ptr<ScriptCollection> collection = std::make_shared<ScriptCollection>();
	collection->AddMember("test", std::make_shared<ScriptInt>(5));
	_globalVariables.emplace(std::make_pair("testCollection", collection));

	AddScript("Scripts/Base/example.vscript");
}

Script* ScriptManager::GetScript(const FS::path & path)
{
	if(path.empty())
	{
		return nullptr;
	}

	auto& iter = _scripts.find(path.string());
	if(iter == _scripts.end())
	{
		return AddScript(path);
	}

	return iter->second.get();
}

Script* ScriptManager::AddScript(const FS::path& path)
{
	std::vector<std::string> lines = ResourceLoader::ReturnFileLines(path, false);
	if(lines.size() > 0)
	{
		const std::string& scriptName = path.string();
		Script* script = _scripts.emplace(std::make_pair(scriptName, std::make_unique<Script>(scriptName, lines))).first->second.get();

		HandleScriptBindings(script);
		script->Init();
		return script;
	}

	return nullptr;
}

void ScriptManager::HandleScriptBindings(Script* script)
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
			script->BindFunction("ve_log",
				[](const Script* sc, ScriptArgumentCollection& args)->std::shared_ptr<BaseScriptVariable>
			{
				return DebugLog::Push(sc, args);
			});
		}
		else if(directive == "Time")
		{
			script->BindFunction("time_frameCount",
				[](const Script*, ScriptArgumentCollection&)->std::shared_ptr<BaseScriptVariable>
			{
				return std::make_shared<ScriptInt>((long)Time::frameCount);
			});
		}
	}
}

void ScriptManager::AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable)
{
	_globalVariables.emplace(name, variable);
}

void ScriptManager::RemoveScript(const FS::path & path)
{
	_scripts.erase(path.string());
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

}

void ScriptManager::Cleanup()
{
	_scripts.clear();
	_globalVariables.clear();
}