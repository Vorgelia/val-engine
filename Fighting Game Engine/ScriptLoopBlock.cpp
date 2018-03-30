#include "ScriptLoopBlock.h"
#include "ScriptParsingUtils.h"
#include "Script.h"

bool ScriptLoopBlock::HandleControlFlag()
{
	switch(_owner->controlFlag())
	{
	case ScriptControlFlag::Continue:
		_cursor = _lines.size();
		_owner->ConsumeControlFlag();
		return false;
	case ScriptControlFlag::Break:
		_owner->ConsumeControlFlag();
	case ScriptControlFlag::Return:
		return true;
	}
	return false;
}

ScriptLoopBlock::ScriptLoopBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner)
	:ScriptConditionalBlock(conditionTokens, lines, depth, parent, owner)
{
}

ScriptLoopBlock::~ScriptLoopBlock()
= default;
