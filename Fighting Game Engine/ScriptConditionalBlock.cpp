#include "ScriptConditionalBlock.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptToken.h"
#include "ScriptVariable.h"
#include "ScriptExpression.h"

bool ScriptConditionalBlock::Evaluate()
{
	std::shared_ptr<BaseScriptVariable> result = ScriptExpression(this, _conditionTokens).Evaluate();
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
	:ScriptBlock(lines, depth, parent, owner), _conditionTokens(conditionTokens)
{
}

ScriptConditionalBlock::~ScriptConditionalBlock()
{
}
