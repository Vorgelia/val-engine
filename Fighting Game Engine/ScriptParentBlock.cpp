#include "CommonUtilIncludes.hpp"
#include "Script.h"
#include "ScriptParsingUtils.h"
#include "ScriptParentBlock.h"
#include "ScriptFunctionBlock.h"
#include "ScriptFunctionSignature.h"
#include "ScriptError.h"
#include <memory>

//Cache line references to functions.
void ScriptParentBlock::HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens)
{
	ScriptFunctionSignature signature = ScriptParsingUtils::ParseFunctionSignature(_lines, _cursor);

	if(_functions.find(signature.name) != _functions.end())
	{
		throw ScriptError("Parser error: Duplicate function definition.");
	}

	_functions[signature.name] = signature;
	_cursor = signature.end + 1;
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

bool ScriptParentBlock::HasFunction(std::string name)
{
	return _functions.find(name) != _functions.end();
}

//Create a new function script block and add some variables to it based on the function signature.
std::shared_ptr<BaseScriptVariable> ScriptParentBlock::RunFunction(std::string name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables)
{
	auto function = _functions.find(name);
	if(function == _functions.end())
	{
		return _owner->CallBoundFunction(name, variables);
	}

	ScriptLinesView functionLines = ScriptLinesView(_lines.lines(), function->second.start + 1, function->second.end + 1);

	std::shared_ptr<ScriptFunctionBlock> scriptBlock = std::make_shared<ScriptFunctionBlock>(&function->second, functionLines, _depth + 1, this, _owner);
	_owner->PushBlock(scriptBlock);

	scriptBlock->Run(variables);
	std::shared_ptr<BaseScriptVariable> returnVal = scriptBlock->returnValue();

	_owner->PopBlock();

	return returnVal;
}

ScriptParentBlock::ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner) :ScriptBlock(lines, depth, nullptr, owner)
{
	ScriptBlock::Run();
}


ScriptParentBlock::~ScriptParentBlock()
= default;
