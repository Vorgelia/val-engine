#pragma once
#include "ScriptBlock.h"
class ScriptConditionalBlock :
	public ScriptBlock
{
	std::vector<ScriptToken>& _conditionTokens;
public:
	bool Evaluate();
	ScriptConditionalBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptConditionalBlock();
};

