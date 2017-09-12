#include "CommonUtilIncludes.hpp"
#include "ScriptParsingUtils.h"
#include "ScriptParentBlock.h"
#include "ScriptFunctionBlock.h"
#include "ScriptFunctionSignature.h"
#include "ScriptError.h"
#include "Script.h"
#include <memory>

//Cache line references to functions.
//Not meant to be any kind of rigorous parsing quite yet, so we can get away with just checking if the indentation level is the same.
void ScriptParentBlock::PreProcess()
{
	for(_cursor = 0; _cursor < _lines.size(); _cursor++)
	{
		int indentation;
		std::string line = ScriptParsingUtils::TrimLine(_lines[_cursor], indentation);
		if(indentation != _depth)
		{
			continue;
		}

		std::vector<std::string> spl;
		boost::split(spl, line, boost::is_any_of(" "), boost::token_compress_on);

		if(spl.size() < 2)
		{
			continue;
		}

		if(spl[0] == "function")
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
	}

	Run();
}

void ScriptParentBlock::HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens)
{
	_cursor = ScriptParsingUtils::FindBlockEnd(_lines, _cursor);
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
void ScriptParentBlock::RunFunction(std::string name)
{
	auto function = _functions.find(name);
	if(function == _functions.end())
	{
		throw ScriptError("Attempting to call invalid function '" + name + "'");
	}

	ScriptLinesView functionLines = ScriptLinesView(_lines.lines(), function->second.start + 1, function->second.end);

	std::shared_ptr<ScriptFunctionBlock> scriptBlock = std::make_shared<ScriptFunctionBlock>(&function->second, functionLines, _depth + 1, this, _owner);

	//TODO: Parse variables, pass to function.
	std::vector<ScriptVariable> variables;
	scriptBlock->Run(variables);
}

ScriptParentBlock::ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner) :ScriptBlock(lines, depth, nullptr, owner)
{
	PreProcess();
}


ScriptParentBlock::~ScriptParentBlock()
{
}
