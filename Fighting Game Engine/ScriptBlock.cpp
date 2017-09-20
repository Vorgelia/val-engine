#include "ScriptBlock.h"
#include "Script.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"

size_t ScriptBlock::cursor(bool absolute)
{
	return _cursor + (absolute ? _lines.front() : 0);
}

void ScriptBlock::ParseLine(const std::string &line)
{
	if(line[0] == '#')
	{
		return;
	}

	std::vector<ScriptToken> tokens;
	ScriptParsingUtils::ParseLineTokens(line, tokens);

	if(tokens.size() == 0)
	{
		return;
	}

	if(tokens[0].token == ScriptToken::function_declaration)
	{
		HandleFunctionDeclarationLine(tokens);
	}
	else if(tokens[0].token == ScriptToken::conditional_declaration)
	{
		HandleConditionalDeclarationLine(tokens);
	}
	else if(tokens[0].token == ScriptToken::while_loop_declaration)
	{
		HandleLoopDeclarationLine(tokens);
	}
	else
	{
		HandleExpressionLine(tokens);
	}
}

void ScriptBlock::HandleExpressionLine(std::vector<ScriptToken> &tokens)
{
	size_t i = 0;
	//something about parse state
	for(; i < tokens.size(); ++i)
	{
		ScriptToken &token = tokens[i];
		if(token.token == ScriptToken::block_return)
		{
			_owner->RaiseControlFlag(ScriptControlFlag::Return);
		}
		else if(token.token == ScriptToken::block_break)
		{
			_owner->RaiseControlFlag(ScriptControlFlag::Break);
			break;
		}
		else if(token.token == ScriptToken::block_continue)
		{
			_owner->RaiseControlFlag(ScriptControlFlag::Continue);
			break;
		}
	}

	if(i < tokens.size())
	{
		throw ScriptError("Parse error: Unexpected token: " + tokens[i].token);
	}
}

void ScriptBlock::HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens)
{
	throw ScriptError("Misplaced function declaration.");
}

void ScriptBlock::HandleLoopDeclarationLine(std::vector<ScriptToken> &tokens)
{

}

void ScriptBlock::HandleConditionalDeclarationLine(std::vector<ScriptToken> &tokens)
{

}

void ScriptBlock::Run()
{
	for(_cursor = 0; _cursor < _lines.size(); ++_cursor)
	{
		int depth;
		std::string line = ScriptParsingUtils::TrimLine(_lines[_cursor], depth);

		if(depth < 0)
		{
			continue;
		}
		else if(depth != _depth)
		{
			throw ScriptError("Parser error: Line of invalid depth within block.");
		}

		ParseLine(line);

		if(HandleControlFlag())
		{
			break;
		}
	}
}

std::shared_ptr<ScriptVariable> ScriptBlock::RunFunction(std::string name, const std::vector<std::shared_ptr<ScriptVariable>> &variables)
{
	if(_parent == nullptr)
	{
		throw ScriptError("Attempting to call invalid function '" + name + "'");
	}
	_parent->RunFunction(name, variables);
}

std::shared_ptr<ScriptVariable> ScriptBlock::GetVariable(std::string name)
{
	auto iter = _variables.find(name);
	if(iter != _variables.end())
	{
		return iter->second;
	}

	if(_parent == nullptr)
	{
		throw ScriptError("Attempting to index invalid variable " + name);
	}

	return _parent->GetVariable(name);
}

bool ScriptBlock::HandleControlFlag()
{
	switch(_owner->controlFlag())
	{
	case ScriptControlFlag::Break:
	case ScriptControlFlag::Return:
	case ScriptControlFlag::Continue:
		return true;
	}
	return false;
}

ScriptBlock::ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner) : _lines(lines)
{
	_depth = depth;
	_parent = parent;
	_owner = owner;
}

ScriptBlock::~ScriptBlock()
{

}
