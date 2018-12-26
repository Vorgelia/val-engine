#pragma once
#include "BaseService.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

namespace fs = std::filesystem;

class Script;
class BaseScriptVariable;
class GameCharacter;
class Debug;
class FilesystemManager;
class ResourceManager;

class ScriptManager : public BaseService
{
	VE_OBJECT_DECLARATION(ScriptManager);

protected:
	Debug* _debug;
	FilesystemManager* _filesystem;
	ResourceManager* _resource;

private:
	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _globalVariables;
	std::unordered_set<std::shared_ptr<Script>> _scripts;
	
	void HandleScriptBindings(Script* script);
	void CacheGlobalVariables();

public:
	void OnInit() override;
	void OnServiceInit() override;
	void OnDestroyed() override;

	Script* GetScript(const fs::path& path);
	Script* AddScript(const fs::path& path);
	void AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable);

	void HandleScriptCharacterBindings(GameCharacter& character, Script* script) const;

	void RemoveScript(Script* script);

	std::shared_ptr<BaseScriptVariable> GetVariable(const std::string& name);

	ScriptManager() = default;
	~ScriptManager() = default;
};