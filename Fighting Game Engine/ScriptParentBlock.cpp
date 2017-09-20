#include "CommonUtilIncludes.hpp"
#include "ScriptParsingUtils.h"
#include "ScriptParentBlock.h"
#include "ScriptFunctionBlock.h"
#include "ScriptFunctionSignature.h"
#include "ScriptError.h"
#include "Script.h"
#include <memory>

//Cache line references to functions.
void ScriptParentBlock::HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens)
{
	ScriptFunctionSignature signature = ScriptParsingUtils::ParseFunctionSignature(_lines, _cursor);

	if(signature.name.empty())
	{
		throw ScriptError("Parser error: Invalid function definition.");
	}

	if(signature.end < 0)
	{
		throw ScriptError("Parser error: Improperly terminated function '" + signature.name + "'.");
	}

	if(_functions.find(signature.name) != _functions.end())
	{
		throw ScriptError("Parser error: Duplicate function definition.");
	}

	_functions[signature.name] = signature;
	_cursor = signature.end;
}

bool ScriptParentBlock::HandleControlFlag()
{
	switch(_owner->controlFlag())
	{
		case ScriptControlFlag::Break:
		case ScriptControlFlag::Return:
		case ScriptControlFlag::Continue:
			throw ScriptError("Invalid declaration of control flag in global scope.");
	}
	return false;
}

//Create a new function script block and add some variables to it based on the function signature.
std::shared_ptr<ScriptVariable> ScriptParentBlock::RunFunction(std::string name, const std::vector<std::shared_ptr<ScriptVariable>> &variables)
{
	auto function = _functions.find(name);
	if(function == _functions.end())
	{
		throw ScriptError("Attempting to call invalid function '" + name + "'");
	}

	ScriptLinesView functionLines = ScriptLinesView(_lines.lines(), function->second.start + 1, function->second.end);

	ScriptFunctionBlock* scriptBlock = new ScriptFunctionBlock(&function->second, functionLines, _depth + 1, this, _owner);
	_owner->PushBlock(scriptBlock);

	scriptBlock->Run(variables);
	std::shared_ptr<ScriptVariable> returnVal = scriptBlock->returnValue();

	_owner->PopBlock();

	return returnVal;
}

ScriptParentBlock::ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner) :ScriptBlock(lines, depth, nullptr, owner)
{
	Run();
}


ScriptParentBlock::~ScriptParentBlock()
{
}
