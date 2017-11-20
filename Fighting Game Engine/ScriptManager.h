#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include<boost\filesystem.hpp>

namespace FS = boost::filesystem;

class Script;
class BaseScriptVariable;
class GameCharacter;

class ScriptManager
{
	static std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _globalVariables;
	static std::unordered_set<std::shared_ptr<Script>> _scripts;
	
	static void HandleScriptBindings(Script* script);
	static void HandleCharacterStateVariables();
public:
	static void Init();
	static void Update();
	static void Cleanup();

	static Script* GetScript(const FS::path& path);
	static Script* AddScript(const FS::path& path);
	static void AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable);

	static void HandleScriptCharacterBindings(GameCharacter& character, Script* script);

	static void RemoveScript(Script* script);

	static std::shared_ptr<BaseScriptVariable> GetVariable(const std::string& name);

};