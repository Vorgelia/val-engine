#pragma once
#include <memory>
#include <string>
#include <vector>
#include <map>

enum class ScriptExitCode;
class ScriptBlock;

class Script
{
	std::string _name;

	std::vector<std::string> _lines;
	std::map<std::string, std::string> _pragmaDirectives;

	ScriptBlock* _block;

	void PreProcess();

public:

	ScriptExitCode Execute();
	ScriptExitCode ExecuteFunction(std::string name);

	Script(std::string name, std::vector<std::string> lines);
	~Script();
};

