#include "Script.h"
#include "ScriptError.h"
#include "ScriptFunctionBlock.h"
#include "ScriptParsingUtils.h"
#include "ScriptFunctionSignature.h"

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

void ScriptFunctionBlock::Run(const std::vector<ScriptVariable> &variables)
{
	//add variables
	ScriptBlock::Run();
}

ScriptFunctionBlock::ScriptFunctionBlock(ScriptFunctionSignature* signature, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner)
: ScriptBlock(lines, depth, parent, owner)
{
	_signature = signature;
	//add __VE_RETURN variable of type signature.returnType
}


ScriptFunctionBlock::~ScriptFunctionBlock()
{
}
