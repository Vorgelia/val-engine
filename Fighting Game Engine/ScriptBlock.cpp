#include "ScriptBlock.h"
#include "Script.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptConditionalBlock.h"
#include "ScriptExpression.h"

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
		int blockEnd;
		HandleConditionalDeclarationLine(tokens, blockEnd);
		_cursor = blockEnd + 1;
	}
	else if(tokens[0].token == ScriptToken::while_loop_declaration)
	{
		int blockEnd = _cursor;
		HandleLoopDeclarationLine(tokens, blockEnd);
		_cursor = blockEnd + 1;
	}
	else
	{
		HandleExpressionLine(tokens);
	}
}

void ScriptBlock::HandleExpressionLine(std::vector<ScriptToken> &tokens)
{
	if(tokens.size() < 1)
	{
		return;
	}

	if(tokens[0].token == ScriptToken::block_break)
	{
		_owner->RaiseControlFlag(ScriptControlFlag::Break);
	}
	else if(tokens[0].token == ScriptToken::block_continue)
	{
		_owner->RaiseControlFlag(ScriptControlFlag::Continue);
	}
	else if(tokens[0].token == ScriptToken::block_return)
	{
		_owner->RaiseControlFlag(ScriptControlFlag::Return);
		_variables[VE_SCRIPT_RETURN_VARIABLE_ID] = EvaluateExpression(std::vector<ScriptToken>(tokens.begin() + 1, tokens.end()));
		return;
	}
	else
	{
		EvaluateExpression(std::vector<ScriptToken>(tokens.begin(), tokens.end()));
		return;
	}

	if(tokens.size() > 1)
	{
		throw ScriptError("Parse error: Unexpected token: " + tokens[1].token);
	}
}

void ScriptBlock::HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens)
{
	throw ScriptError("Misplaced function declaration.");
}

void ScriptBlock::HandleLoopDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd)
{
	std::vector<ScriptToken> parenthesisTokens;
	int blockEnd;
	ScriptParsingUtils::ParseConditionalExpression(_lines, std::forward<std::vector<ScriptToken>>(tokens), _cursor, parenthesisTokens, blockEnd);

	out_blockEnd = blockEnd;
	ScriptLinesView blockLines = ScriptLinesView(_lines, _cursor + 1, out_blockEnd);

	bool validExpression;
	do
	{
		std::shared_ptr<ScriptConditionalBlock> block = std::make_shared<ScriptConditionalBlock>(parenthesisTokens, blockLines, _depth + 1, this, _owner);
		_owner->PushBlock(block);

		validExpression = block->Evaluate();

		_owner->PopBlock();
	} while(validExpression);
}

void ScriptBlock::HandleConditionalDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd)
{
	bool branchRan = false;
	bool initialBranchChecked = false;

	out_blockEnd = _cursor;

	while(tokens.size() > 0)
	{
		if(tokens[0].token == ScriptToken::conditional_else)
		{
			if(tokens.size() > 1)
			{
				throw ScriptError("Unexpected token " + tokens[1].token);
			}

			out_blockEnd = ScriptParsingUtils::FindBlockEnd(_lines, out_blockEnd);
			if(!branchRan)
			{
				ScriptLinesView blockLines = ScriptLinesView(_lines, _cursor + 1, out_blockEnd + 1);
				std::shared_ptr<ScriptBlock> block = std::make_shared<ScriptBlock>(blockLines, _depth + 1, this, _owner);
				block->Run();
			}
			return;
		}
		else if(tokens[0].token == ScriptToken::conditional_elseif || (tokens[0].token == ScriptToken::conditional_declaration && !initialBranchChecked))
		{
			initialBranchChecked = true;

			if(branchRan)
			{
				out_blockEnd = ScriptParsingUtils::FindBlockEnd(_lines, out_blockEnd);
			}
			else
			{
				std::vector<ScriptToken> parenthesisTokens;
				ScriptParsingUtils::ParseConditionalExpression(_lines, std::forward<std::vector<ScriptToken>>(tokens), _cursor, parenthesisTokens, out_blockEnd);

				ScriptLinesView blockLines = ScriptLinesView(_lines, _cursor + 1, out_blockEnd + 1);
				std::shared_ptr<ScriptConditionalBlock> block = std::make_shared<ScriptConditionalBlock>(parenthesisTokens, blockLines, _depth + 1, this, _owner);

				_owner->PushBlock(block);
				branchRan = block->Evaluate();
				_owner->PopBlock();
			}

			if(out_blockEnd + 2 >= _lines.lines()->size())
			{
				break;
			}

			out_blockEnd += 2;
			ScriptParsingUtils::ParseLineTokens(_lines[out_blockEnd], tokens);
			_cursor = out_blockEnd;
		}
		else
		{
			break;
		}
	}
}

std::shared_ptr<BaseScriptVariable> ScriptBlock::EvaluateExpression(std::vector<ScriptToken>& tokens)
{
	return std::make_shared<ScriptExpression>(this, tokens)->Evaluate();
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

std::shared_ptr<BaseScriptVariable> ScriptBlock::RunFunction(std::string name, const std::vector<std::shared_ptr<BaseScriptVariable>> &variables)
{
	if(_parent == nullptr)
	{
		throw ScriptError("Attempting to call invalid function '" + name + "'");
	}
	return _parent->RunFunction(name, variables);
}

void ScriptBlock::AddVariable(std::string name, std::shared_ptr<BaseScriptVariable> variable)
{
	if(_variables.find(name) != _variables.end())
	{
		throw ScriptError("Attempting to add variable with duplicate name " + name);
	}

	//TODO: Check if name is a keyword

	_variables.emplace(name, variable);
}

std::shared_ptr<BaseScriptVariable> ScriptBlock::GetVariable(std::string name)
{
	auto iter = _variables.find(name);
	if(iter != _variables.end())
	{
		return iter->second;
	}

	if(_parent == nullptr)
	{
		return _owner->GetGlobalVariable(name);
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
