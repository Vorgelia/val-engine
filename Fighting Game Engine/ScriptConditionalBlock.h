#pragma once
#include "ScriptBlock.h"
#include "ScriptExpression.h"

class ScriptConditionalBlock :
	public ScriptBlock
{
	ScriptExpression _expression;
public:

	bool Evaluate();
	virtual bool HandleConditionResult(std::shared_ptr<BaseScriptVariable> result);

	ScriptConditionalBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptConditionalBlock();
};

