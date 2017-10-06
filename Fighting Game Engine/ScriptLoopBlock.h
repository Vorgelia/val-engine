#pragma once
#include "ScriptConditionalBlock.h"
class ScriptLoopBlock :
	public ScriptConditionalBlock
{
public:

	bool HandleControlFlag() override;

	ScriptLoopBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptLoopBlock();
};

