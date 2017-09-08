#include "Script.h"
#include "ScriptError.h"
#include "ScriptFunctionBlock.h"
#include "ScriptParsingUtils.h"

bool ScriptFunctionBlock::HandleControlFlag()
{
	switch(_owner->controlFlag())
	{
		case ScriptControlFlag::Return:
			_owner->ConsumeControlFlag();
			return true;
		case ScriptControlFlag::Break:
		case ScriptControlFlag::Continue:
			throw ScriptError("Invalid script control flag inside function.");
	}
	return false;
}

ScriptFunctionBlock::ScriptFunctionBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner) :ScriptBlock(lines, depth, parent, owner)
{
}


ScriptFunctionBlock::~ScriptFunctionBlock()
{
}
