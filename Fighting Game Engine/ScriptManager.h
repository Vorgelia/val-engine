#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include<boost\filesystem.hpp>

namespace FS = boost::filesystem;

class Script;
class BaseScriptVariable;

class ScriptManager
{
	static std::unordered_map<std::string, std::unique_ptr<Script>> _scripts;
	static std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _globalVariables;

	static void HandleScriptBindings(Script* script);

public:
	static void Init();
	static void Update();
	static void Cleanup();

	static Script* AddScript(const FS::path& path);
	static void AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable);

	static void RemoveScript(const FS::path& path);

	static std::shared_ptr<BaseScriptVariable> GetVariable(const std::string& name);

};