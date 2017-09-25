#include "ScriptConditionalBlock.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptToken.h"

bool ScriptConditionalBlock::Evaluate()
{
	//if evaluateexpression conditionTokens is boolean and true
	//	run
	return true;
	//else
	//return false
}

ScriptConditionalBlock::ScriptConditionalBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner)
	:ScriptBlock(lines, depth, parent, owner), _conditionTokens(conditionTokens)
{
}

ScriptConditionalBlock::~ScriptConditionalBlock()
{
}
