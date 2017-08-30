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

	int _depth;

	int _originLine;
	std::vector<std::string> _lines;

	std::map<std::string, int> _functions;

	void PreProcess();
public:
	
	void Run();
	void RunFunction(std::string name, ...);
	ScriptBlock(std::vector<std::string> lines, int originLine, int depth, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

