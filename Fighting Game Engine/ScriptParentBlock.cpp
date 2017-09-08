#include "CommonUtilIncludes.hpp"
#include "ScriptParsingUtils.h"
#include "ScriptParentBlock.h"
#include "ScriptFunctionSignature.h"
#include "ScriptError.h"
#include "Script.h"
#include <memory>

//Cache line references to functions and variables.
//Not meant to be any kind of rigorous parsing quite yet, so we can get away with just checking if the indentation level is the same.
void ScriptParentBlock::PreProcess()
{
	for(size_t i = 0; i < _lines.size(); i++)
	{
		int indentation;
		std::string line = ScriptParsingUtils::TrimLine(_lines[i], indentation);
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
			ScriptFunctionSignature signature = ScriptParsingUtils::ParseFunctionSignature(_lines, i);

			if(signature.name.empty())
			{
				throw ScriptError("Parser error: Invalid function definition.", _owner->name(), _lines.front() + i);
			}

			if(signature.end < 0)
			{
				throw ScriptError("Parser error: Improperly terminated function '" + signature.name + "'.", _owner->name(), _lines.front() + i);
			}

			if(_functions.find(signature.name) != _functions.end())
			{
				throw ScriptError("Parser error: Duplicate function definition.", _owner->name(), _lines.front() + i);
			}

			_functions[signature.name] = signature;
			i = signature.end;
		}
	}

	Run();
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
		throw ScriptError("Attempting to call invalid function '" + name + "'", _owner->name(), -1);
	}

	ScriptLinesView functionLines = ScriptLinesView(_lines.lines(), function->second.start + 1, function->second.end);

	std::shared_ptr<ScriptBlock> scriptBlock = std::make_shared<ScriptBlock>(functionLines, _depth + 1, this, _owner);
	scriptBlock->Run();
}

ScriptParentBlock::ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner) :ScriptBlock(lines, depth, nullptr, owner)
{
	PreProcess();
}


ScriptParentBlock::~ScriptParentBlock()
{
}
