#include "Script.h"
#include "ScriptVariable.h"
#include "ScriptParsingUtils.h"
#include "CommonUtilIncludes.hpp"
#include "ScriptError.h"
#include "ScriptParentBlock.h"
#include "DebugLog.h"

std::string Script::name() const
{
	return _name;
}

bool Script::valid() const
{
	return _valid;
}

ScriptControlFlag Script::controlFlag()
{
	return _controlFlag;
}

void Script::BindFunction(std::string name, void(*func)(std::vector<std::shared_ptr<ScriptVariable>>&))
{
	_boundFunctions[name] = func;
}

bool Script::CallBoundFunction(std::string name, std::vector<std::shared_ptr<ScriptVariable>> &variables)
{
	auto iter = _boundFunctions.find(name);
	if(iter != _boundFunctions.end() && iter->second != nullptr)
	{
		iter->second(variables);
		return true;
	}
	return false;
}

void Script::PreProcess()
{
	for(size_t i = 0; i < _lines.size(); ++i)
	{
		int indentation;
		std::string line = ScriptParsingUtils::TrimLine(_lines[i], indentation);
		if(indentation != 0 || line[0] != '#')
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
	}
}

void Script::PushBlock(ScriptBlock* block)
{
	_blockStack.push(block);
}

void Script::PopBlock()
{
	delete _blockStack.top();
	_blockStack.pop();
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
	ScriptExitCode returnCode = ScriptExitCode::Success;

	try
	{
		ExecuteFunction("Main", std::vector<std::shared_ptr<ScriptVariable>>());
	}
	catch(ScriptError error)
	{
		DebugLog::Push("(" + _name + " : line " + std::to_string(_blockStack.top()->cursor()) + ")" + std::string(error.what()), LogItem::Type::Error);
		_valid = false;
		returnCode = ScriptExitCode::Failure;
	}
	catch(std::exception error)
	{
		DebugLog::Push("Unhandled exception on script[" + _name + "]:\n" + std::string(error.what()), LogItem::Type::Error);
		_valid = false;
		returnCode = ScriptExitCode::Exception;
	}

	if(_blockStack.size() > 0)
	{
		DebugLog::Push("Block stack not empty after script " + _name + " execution.", LogItem::Type::Error);
		while(_blockStack.size() > 0)
		{
			PopBlock();
		}
	}

	return returnCode;
}

void Script::ExecuteFunction(std::string name, const std::vector<std::shared_ptr<ScriptVariable>> &variables)
{
	_parentBlock->RunFunction(name, variables);
}

Script::Script(std::string name, std::vector<std::string> lines)
{
	_name = name;
	_lines = lines;
	_valid = true;

	PreProcess();

	_parentBlock = new ScriptParentBlock(ScriptLinesView(&_lines), 0, this);
	ExecuteFunction("Init", std::vector<std::shared_ptr<ScriptVariable>>());
}

Script::~Script()
{
	delete _parentBlock;
}
