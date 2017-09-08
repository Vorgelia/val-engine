#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

enum class ScriptExitCode;
enum class ScriptControlFlag;
class ScriptParentBlock;

class Script
{
	friend class ScriptBlock;

	std::string _name;
	bool _valid;

	std::vector<std::string> _lines;
	std::map<std::string, std::string> _pragmaDirectives;

	ScriptControlFlag _controlFlag;

	ScriptParentBlock* _parentBlock;

	void PreProcess();

public:
	std::string name() const;
	ScriptControlFlag controlFlag();

	void RaiseControlFlag(ScriptControlFlag flag);
	void ConsumeControlFlag();

	ScriptExitCode Execute();
	ScriptExitCode ExecuteFunction(std::string name);

	Script(std::string name, std::vector<std::string> lines);
	~Script();
};

