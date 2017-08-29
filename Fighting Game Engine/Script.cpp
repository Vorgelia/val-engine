#include "Script.h"
#include "ScriptParsingUtils.h"
#include "CommonUtilIncludes.hpp"
#include "ScriptError.h"
#include "ScriptBlock.h"
#include "DebugLog.h"

void Script::PreProcess()
{
	for(int i = 0; i < _lines.size(); ++i)
	{
		int indentation;
		std::string line = ScriptParsingUtils::TrimLine(_lines[i], indentation);
		if(indentation != 0)
		{
			continue;
		}

		std::vector<std::string> spl;
		boost::split(spl, line, boost::is_any_of(" "), boost::token_compress_on);

		if(spl.size() < 2)
		{
			continue;
		}

		if(spl[0] == "#pragma")
		{
			_pragmaDirectives[spl[1]] = spl.size() < 3 ? "" : spl[2];
		}
		else if(spl[0] == "class")
		{
			//TODO: Parse and store Class
		}
	}
}

ScriptExitCode Script::Execute()
{
	return ExecuteFunction("Main");
}

ScriptExitCode Script::ExecuteFunction(std::string name)
{
	try
	{
		_block->RunFunction(name);
	}
	catch(ScriptError error)
	{
		DebugLog::Push(error.what());
		return ScriptExitCode::Failure;
	}
	catch(std::exception error)
	{
		DebugLog::Push("Unhandled exception on script[" + _name + "]:\n" + std::string(error.what()));
		return ScriptExitCode::Exception;
	}
	return ScriptExitCode::Success;
}

Script::Script(std::string name, std::vector<std::string> lines)
{
	_name = name;
	_lines = lines;

	PreProcess();

	_block = new ScriptBlock(_lines, 0, nullptr, this);
	ExecuteFunction("Init");
}

Script::~Script()
{
	delete _block;
}
