#include "ScriptManager.h"
#include "Script.h"
#include "BaseScriptVariable.h"
#include "ResourceLoader.h"
#include <map>

namespace ScriptManager
{
	std::map<std::string, std::shared_ptr<Script>> _scripts;
	std::map<std::string, std::shared_ptr<BaseScriptVariable>> _globalVariables;
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
		_scripts.emplace(scriptName, std::make_shared<Script>(scriptName, lines));
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