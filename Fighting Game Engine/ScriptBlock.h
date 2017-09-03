#pragma once
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>
#include "ScriptLinesView.h"


class Script;

class ScriptBlock
{
	Script* _owner;
	ScriptBlock* _parent;

	int _depth;

	ScriptLinesView _lines;

	std::map<std::string, int> _functions;

	void PreProcess();
public:
	
	void Run();
	void RunFunction(std::string name, ...);
	ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

