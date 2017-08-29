#include "ScriptBlock.h"
#include "CommonUtilIncludes.hpp"
#include "Script.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"

//PreProcess global block only. Cache line references to functions and variables.
//Not meant to be any kind of rigorous parsing quite yet, so we can get away with just checking if the indentation level is the same.
void ScriptBlock::PreProcess()
{
	if(_parent != nullptr)
	{
		//We might want to do something here later.
		return;
	}

	for(size_t i = 0; i < _lines.size(); i++)
	{
		int indentation;
		std::string line = ScriptParsingUtils::TrimLine(_lines[i], indentation);
		if(indentation != _level)
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
			_functions[spl[1]] = i;
		}
	}

	Run();
}

void ScriptBlock::Run()
{
	//run normally
	//if parent isn't null, throw exceptions on class and function declarations
	//otherwise skip to end of those blocks and continue
}

//Create a new function script block and add some variables to it based on the function signature.
void ScriptBlock::RunFunction(std::string name, ...)
{
	auto functionLine = _functions.find(name);
	if(functionLine == _functions.end())
	{
		throw ScriptError("Attempting to call invalid function '" + name + "\'");
	}

	int functionEndLine = ScriptParsingUtils::FindBlockEnd(_lines, functionLine->second);
	if(functionEndLine < 0)
	{
		throw ScriptError("Function '" + name + "\' not properly terminated.");
	}

	auto fBegin = _lines.begin() + functionLine->second;
	auto fEnd = _lines.begin() + functionEndLine;
	std::vector<std::string> functionLines = std::vector<std::string>(fBegin, fEnd);
	std::shared_ptr<ScriptBlock> scriptBlock = std::make_shared<ScriptBlock>(functionLines, _level + 1, this, _owner);
	scriptBlock->Run();
}

ScriptBlock::ScriptBlock(std::vector<std::string> lines, int level, ScriptBlock* parent, Script* owner)
{
	_lines = lines;
	_level = level;
	_parent = parent;
	_owner = owner;

	PreProcess();
}

ScriptBlock::~ScriptBlock()
{
}
