#pragma once
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>
#include "ScriptLinesView.h"
#include "ScriptVariable.h"

class Script;

class ScriptBlock
{
protected:

	Script* _owner;
	ScriptBlock* _parent;

	int _depth;

	ScriptLinesView _lines;

	size_t _cursor;

	void ParseLine(const std::string &line, int lineIndex);
	virtual bool HandleControlFlag();
public:
	size_t cursor();

	virtual void Run();

	ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

