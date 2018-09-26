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
#include "GameInstance.h"

std::string Script::name() const
{
	return _name;
}

bool Script::valid() const
{
	return _valid;
}

ScriptControlFlag Script::controlFlag() const
{
	return _controlFlag;
}

bool Script::HasFunction(const std::string& name) const
{
	return _parentBlock != nullptr && _parentBlock->HasFunction(name);
}

void Script::BindFunction(const std::string& name, std::function<std::shared_ptr<BaseScriptVariable>(const Script*, ScriptArgumentCollection&)> func)
{
	_boundFunctions.insert_or_assign(name, func);
}

std::shared_ptr<BaseScriptVariable> Script::CallBoundFunction(const std::string& name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables)
{
	auto iter = _boundFunctions.find(name);
	if(iter == _boundFunctions.end() || iter->second == nullptr)
	{
		throw ScriptError("Attempting to call invalid external function '" + name + "'");
	}
	return iter->second(this, variables);
}

std::vector<std::string> Script::GetPragmaDirectives(const std::string& id)
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
	catch(ScriptError& error)
	{
		_debug->VE_LOG("Could not find variable " + name + " in script " + _name + ".\n" + error.what(), LogItem::Type::Warning);
		return nullptr;
	}
}

void Script::AddVariable(const std::string& name, std::shared_ptr<BaseScriptVariable> variable, bool allowReplace)
{
	_parentBlock->AddVariable(name, variable, allowReplace);
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

		if(indentation >= 0 && !tokens.empty() && tokens[0].type != ScriptTokenType::Preprocessor)
		{
			_lines.emplace_back(ScriptLine(line, i, indentation, tokens));
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
	std::shared_ptr<BaseScriptVariable> var = _scriptManager->GetVariable(name);
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
		_parentBlock = std::make_shared<ScriptParentBlock>(ScriptLinesView(&_lines), 0, this);
	}
	catch(ScriptError& error)
	{
		_debug->VE_LOG("(Preprocessing " + _name + ") " + std::string(error.what()), LogItem::Type::Warning);
		_valid = false;
	}
}

void Script::Execute()
{
	ExecuteFunction("Main", std::vector<std::shared_ptr<BaseScriptVariable>>());
}

std::shared_ptr<BaseScriptVariable> Script::ExecuteFunction(const std::string& name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables)
{
	const size_t blockStackSize = _blockStack.size();

	try
	{
		return _parentBlock->RunFunction(name, variables);
	}
	catch(ScriptError& error)
	{
		const int blockCursor = _blockStack.empty() ? _parentBlock->cursor() : _blockStack.top()->cursor();

		_debug->VE_LOG("(" + _name + " : line " + std::to_string(_lines[blockCursor].index) + ") " + std::string(error.what()), LogItem::Type::Warning);
		_valid = false;
	}
	catch(std::exception& error)
	{
		_debug->VE_LOG("Unhandled exception on script[" + _name + "]:\n" + std::string(error.what()), LogItem::Type::Error);
		_valid = false;
	}

	if(blockStackSize != _blockStack.size())
	{
		_debug->VE_LOG("Function " + name + " execution did not properly clear the block stack.", LogItem::Type::Error);
		while(blockStackSize < _blockStack.size())
		{
			_blockStack.pop();
		}
	}

	return nullptr;
}

Script::Script(const std::string& name, std::vector<std::string> lines, GameInstance* serviceManager)
{
	_serviceManager = serviceManager;
	_debug = serviceManager->Debug();
	_scriptManager = serviceManager->ScriptManager();

	_name = name;
	_rawLines = lines;
	_valid = true;

	PreProcess();
}

Script::~Script()
= default;
