#include "Script.h"
#include "ScriptError.h"
#include "ScriptFunctionBlock.h"
#include "ScriptParsingUtils.h"
#include "ScriptFunctionSignature.h"

#define FMT_HEADER_ONLY 1
#include <fmt\format.h>

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

const std::shared_ptr<ScriptVariable> ScriptFunctionBlock::returnValue() const
{
	return _variables.at("__VE_RETURN");
}

void ScriptFunctionBlock::Run(const std::vector<std::shared_ptr<ScriptVariable>> &variables)
{
	if(variables.size() != _signature->arguments.size())
	{
		throw ScriptError(fmt::format("Runtime Error: Mismatched arguments on {}() call. (expected {}, got{})", _signature->name, _signature->arguments.size(), variables.size()));
	}

	/*for(size_t i = 0; i < variables.size(); i++)
	{
		if(_signature->arguments[i].type != variables[i]->type())
		{
			throw ScriptError(fmt::format("Argument of invalid type on {}() call. (expected {}, got {}",_signature->name, _signature->arguments[i].type(), variables[i].type()));
		}
		_variables[_signature->arguments[i].name] = variables[i];
	}*/
	//add variables
	ScriptBlock::Run();
}

ScriptFunctionBlock::ScriptFunctionBlock(ScriptFunctionSignature* signature, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner)
	: ScriptBlock(lines, depth, parent, owner)
{
	_signature = signature;
	_variables["__VE_RETURN"] = std::make_shared<ScriptVariable>(signature->returnType);
}


ScriptFunctionBlock::~ScriptFunctionBlock()
{
}
