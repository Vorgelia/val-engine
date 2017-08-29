#pragma once
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>

class Script;

class ScriptBlock
{
	Script* _owner;
	ScriptBlock* _parent;

	int _level;

	std::vector<std::string> _lines;

	std::map<std::string, int> _functions;

	void PreProcess();
public:
	
	void Run();
	void RunFunction(std::string name, ...);
	ScriptBlock(std::vector<std::string> lines, int level, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

