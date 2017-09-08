#include "Script.h"
#include "ScriptParsingUtils.h"
#include "CommonUtilIncludes.hpp"
#include "ScriptError.h"
#include "ScriptParentBlock.h"
#include "DebugLog.h"

std::string Script::name() const
{
	return _name;
}

ScriptControlFlag Script::controlFlag()
{
	return _controlFlag;
}

void Script::PreProcess()
{
	for(size_t i = 0; i < _lines.size(); ++i)
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

void Script::RaiseControlFlag(ScriptControlFlag flag)
{
	_controlFlag = flag;
}

void Script::ConsumeControlFlag()
{
	_controlFlag = ScriptControlFlag::None;
}

ScriptExitCode Script::Execute()
{
	return ExecuteFunction("Main");
}

ScriptExitCode Script::ExecuteFunction(std::string name)
{
	try
	{
		_parentBlock->RunFunction(name);
	}
	catch(ScriptError error)
	{
		DebugLog::Push(error.what());
		_valid = false;
		return ScriptExitCode::Failure;
	}
	catch(std::exception error)
	{
		DebugLog::Push("Unhandled exception on script[" + _name + "]:\n" + std::string(error.what()));
		_valid = false;
		return ScriptExitCode::Exception;
	}
	return ScriptExitCode::Success;
}

Script::Script(std::string name, std::vector<std::string> lines)
{
	_name = name;
	_lines = lines;

	PreProcess();

	_parentBlock = new ScriptParentBlock(ScriptLinesView(&_lines), 0, this);
	ExecuteFunction("Init");
}

Script::~Script()
{
	delete _parentBlock;
}
