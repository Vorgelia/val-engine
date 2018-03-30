#include "ScriptConditionalBlock.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptToken.h"
#include "ScriptVariable.h"
#include "ScriptExpression.h"
#include "Script.h"

bool ScriptConditionalBlock::Evaluate()
{
	std::shared_ptr<BaseScriptVariable> result = _expression.Evaluate();
	return HandleConditionResult(result);
}

bool ScriptConditionalBlock::HandleConditionResult(std::shared_ptr<BaseScriptVariable> result)
{
	if(result->type() != ScriptVariableType::Bool)
	{
		throw ScriptError("Invalid result type in conditional block expression.");
	}

	if(std::static_pointer_cast<ScriptBool>(result)->value())
	{
		Run();
		return true;
	}
	else
	{
		return false;
	}
}

ScriptConditionalBlock::ScriptConditionalBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner)
	:ScriptBlock(lines, depth, parent, owner),_expression(this, conditionTokens)
{
}

ScriptConditionalBlock::~ScriptConditionalBlock()
= default;
