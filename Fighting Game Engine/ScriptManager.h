#pragma once
#include "BaseService.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include<boost\filesystem.hpp>

namespace FS = boost::filesystem;

class Script;
class BaseScriptVariable;
class GameCharacter;
class Debug;

class ScriptManager : public BaseService
{
	Debug* _debug;

private:
	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _globalVariables;
	std::unordered_set<std::shared_ptr<Script>> _scripts;
	
	void HandleScriptBindings(Script* script);
	void HandleCharacterStateVariables();

public:
	void Update();

	Script* GetScript(const FS::path& path);
	Script* AddScript(const FS::path& path);
	void AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable);

	void HandleScriptCharacterBindings(GameCharacter& character, Script* script);

	void RemoveScript(Script* script);

	std::shared_ptr<BaseScriptVariable> GetVariable(const std::string& name);

	ScriptManager(ServiceManager* serviceManager);
	~ScriptManager();
};