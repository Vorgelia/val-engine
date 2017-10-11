#pragma once
#include <memory>
#include <string>
#include<boost\filesystem.hpp>

namespace FS = boost::filesystem;

class Script;
class BaseScriptVariable;

namespace ScriptManager
{
	void Init();

	void AddScript(const FS::path& path);
	void AddVariable(const std::string& name, const std::shared_ptr<BaseScriptVariable>& variable);

	std::shared_ptr<BaseScriptVariable> GetVariable(const std::string& name);

	void Update();
	void Cleanup();
}
