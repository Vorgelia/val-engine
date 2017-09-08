#pragma once
#include "ScriptBlock.h"
#include <string>
#include <stdarg.h>

class ScriptFunctionBlock :
	public ScriptBlock
{
	//ScriptFunctionSignature& _signature;

	bool HandleControlFlag();
public:
	const std::string& returnValue() const;

	void Run(std::vector<ScriptVariable> variables);

	ScriptFunctionBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptFunctionBlock();
};

