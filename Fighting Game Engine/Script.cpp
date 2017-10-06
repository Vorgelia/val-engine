#include "Script.h"
#include "ScriptManager.h"
#include "BaseScriptVariable.h"
#include "ScriptParsingUtils.h"
#include "CommonUtilIncludes.hpp"
#include "ScriptError.h"
#include "ScriptParentBlock.h"
#include "DebugLog.h"
#include "ScriptLine.h"
#include "ScriptToken.h"

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

void Script::BindFunction(std::string name, std::shared_ptr<BaseScriptVariable>(*func)(const Script*, std::vector<std::shared_ptr<BaseScriptVariable>>&))
{
	_boundFunctions[name] = func;
}

std::shared_ptr<BaseScriptVariable> Script::CallBoundFunction(std::string name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables)
{
	auto iter = _boundFunctions.find(name);
	if(iter == _boundFunctions.end() || iter->second == nullptr)
	{
		throw ScriptError("Attempting to call invalid external function '" + name + "'");
	}
	return iter->second(this, variables);
}

std::vector<std::string> Script::GetPragmaDirectives(std::string id)
{
	const auto& iter = _pragmaDirectives.find(id);
	if(iter != _pragmaDirectives.end())
	{
		return iter->second;
	}
	return std::vector<std::string>();
}

std::shared_ptr<BaseScriptVariable> Script::GetVariable(const std::string& name) const
{
	try
	{
		return _parentBlock->GetVariable(name);
	}
	catch(ScriptError error)
	{
		DebugLog::Push("Could not find variable " + name + " in script " + _name + ".", LogItem::Type::Error);
		return nullptr;
	}
}

void Script::PreProcess()
{
	_lines.reserve(_rawLines.size());
	for(size_t i = 0; i < _rawLines.size(); ++i)
	{
		int indentation;
		std::string line = ScriptParsingUtils::TrimLine(_rawLines[i], indentation);

		std::vector<ScriptToken> tokens;
		ScriptParsingUtils::ParseLineTokens(line, tokens);

		if(indentation >= 0 && tokens.size() > 0 && tokens[0].type != ScriptTokenType::Preprocessor)
		{
			_lines.push_back(ScriptLine(line, i, indentation, tokens));
		}

		if(indentation != 0 || tokens.size() < 3 || tokens[0].type != ScriptTokenType::Preprocessor || tokens[1].token != ScriptToken::preprocessor_pragma)
		{
			continue;
		}

		std::string directive = tokens.size() < 4 ? "" : tokens[3].token;

		const auto& iter = _pragmaDirectives.find(tokens[2].token);
		if(iter == _pragmaDirectives.end())
		{
			_pragmaDirectives.emplace(tokens[2].token, std::vector<std::string>{directive});
		}
		else
		{
			iter->second.push_back(directive);
		}
	}
}

void Script::PushBlock(std::shared_ptr<ScriptBlock> block)
{
	_blockStack.push(block);
}

void Script::PopBlock()
{
	_blockStack.pop();
}

std::shared_ptr<BaseScriptVariable> Script::GetGlobalVariable(const std::string & name) const
{
	std::shared_ptr<BaseScriptVariable> var = ScriptManager::GetVariable(name);
	if(var == nullptr)
	{
		throw ScriptError("Attempting to index invalid variable " + name);
	}
	return var;
}

void Script::RaiseControlFlag(ScriptControlFlag flag)
{
	_controlFlag = flag;
}

void Script::ConsumeControlFlag()
{
	_controlFlag = ScriptControlFlag::None;
}

void Script::Init()
{
	try
	{
		_parentBlock = new ScriptParentBlock(ScriptLinesView(&_lines), 0, this);
		ExecuteFunction("Init", std::vector<std::shared_ptr<BaseScriptVariable>>());
	}
	catch(ScriptError error)
	{
		DebugLog::Push("(" + _name + " : line " + std::to_string(_lines[_blockStack.top()->cursor()].index) + ")" + std::string(error.what()), LogItem::Type::Error);
		_valid = false;
	}

	if(_blockStack.size() > 0)
	{
		DebugLog::Push("Block stack not empty after script " + _name + " execution.", LogItem::Type::Error);
		while(_blockStack.size() > 0)
		{
			PopBlock();
		}
	}
}

ScriptExitCode Script::Execute()
{
	ScriptExitCode returnCode = ScriptExitCode::Success;

	try
	{
		ExecuteFunction("Main", std::vector<std::shared_ptr<BaseScriptVariable>>());
	}
	catch(ScriptError error)
	{
		DebugLog::Push("(" + _name + " : line " + std::to_string(_lines[_blockStack.top()->cursor()].index) + ")" + std::string(error.what()), LogItem::Type::Error);
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

void Script::ExecuteFunction(std::string name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables)
{
	_parentBlock->RunFunction(name, variables);
}

Script::Script(std::string name, std::vector<std::string> lines)
{
	_name = name;
	_rawLines = lines;
	_valid = true;

	PreProcess();
}

Script::~Script()
{
	delete _parentBlock;
}
