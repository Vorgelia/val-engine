#include "ScriptBlock.h"
#include "Script.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptConditionalBlock.h"

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
		bool branchRan = false;
		while(tokens.size() > 0
			&& (tokens[0].token == ScriptToken::conditional_declaration || tokens[0].token == ScriptToken::conditional_else))
		{
			int blockEnd;
			if(!branchRan)
			{
				branchRan = HandleConditionalDeclarationLine(tokens, blockEnd);
			}
			else
			{
				blockEnd = ScriptParsingUtils::FindBlockEnd(_lines, _cursor);
			}

			_cursor = blockEnd + 1;
			if(_cursor >= _lines.size())
			{
				break;
			}

			ScriptParsingUtils::ParseLineTokens(_lines[_cursor], tokens);
		}
	}
	else if(tokens[0].token == ScriptToken::while_loop_declaration)
	{
		int blockEnd = _cursor;
		HandleLoopDeclarationLine(tokens, blockEnd);
		_cursor = blockEnd;
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
			if(i > 0)
			{
				break;
			}
			_owner->RaiseControlFlag(ScriptControlFlag::Return);
			_variables[VE_SCRIPT_RETURN_VARIABLE_ID] = EvaluateExpression(std::vector<ScriptToken>(tokens.begin() + 1, tokens.end()));
		}
		else if(token.token == ScriptToken::block_break)
		{
			if(i > 0)
			{
				break;
			}
			_owner->RaiseControlFlag(ScriptControlFlag::Break);
			break;
		}
		else if(token.token == ScriptToken::block_continue)
		{
			if(i > 0)
			{
				break;
			}
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

void ScriptBlock::HandleLoopDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd)
{
	std::vector<ScriptToken> parenthesisTokens;
	int blockEnd;
	ScriptParsingUtils::ParseConditionalExpression(_lines, std::forward<std::vector<ScriptToken>>(tokens), _cursor, parenthesisTokens, blockEnd);

	out_blockEnd = blockEnd;
	ScriptLinesView blockLines = ScriptLinesView(_lines.lines(), _cursor + 1, out_blockEnd);
	
	bool validExpression;
	do
	{
		std::shared_ptr<ScriptConditionalBlock> block = std::make_shared<ScriptConditionalBlock>(parenthesisTokens, blockLines, _depth + 1, this, _owner);
		_owner->PushBlock(block);

		validExpression = block->Evaluate();

		_owner->PopBlock();
	} while(validExpression);
}

bool ScriptBlock::HandleConditionalDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd)
{
	std::vector<ScriptToken> parenthesisTokens;
	int blockEnd;
	ScriptParsingUtils::ParseConditionalExpression(_lines, std::forward<std::vector<ScriptToken>>(tokens), _cursor, parenthesisTokens, blockEnd);

	out_blockEnd = blockEnd;
	ScriptLinesView blockLines = ScriptLinesView(_lines.lines(), _cursor + 1, out_blockEnd);
	std::shared_ptr<ScriptConditionalBlock> block = std::make_shared<ScriptConditionalBlock>(parenthesisTokens, blockLines, _depth + 1, this, _owner);

	_owner->PushBlock(block);

	bool blockExecuted = block->Evaluate();

	_owner->PopBlock();

	return blockExecuted;
}

std::shared_ptr<BaseScriptVariable> ScriptBlock::EvaluateExpression(std::vector<ScriptToken>& tokens)
{
	//operator stack
	//variable stack

	//for(size_t i = 0; i < tokens.size(); ++i)
	//{
	//	ScriptToken& token = tokens[i];
	//	//switch parsing state
	//	switch(token.type)
	//	{
	//	case ScriptTokenType::ParenthesisGroup:
	//	{
	//		std::vector<ScriptToken> parenthesisTokens;
	//		ScriptParsingUtils::ParseLineTokens(token.token, parenthesisTokens);
	//		std::shared_ptr<BaseScriptVariable> result = EvaluateExpression(parenthesisTokens);
	//		//push result to variable stack
	//	}
	//	break;
	//	case ScriptTokenType::Operator:
	//	{
	//		//check if valid operator
	//		//while priority < operator stack top
	//		//apply operator stack top
	//		//push to operator stack
	//	}
	//	break;
	//	case ScriptTokenType::NumericLiteral:
	//	{
	//		//push int variable to operator stack
	//	}
	//	break;
	//	case ScriptTokenType::Keyword:
	//	{
	//		//if the keyword is a type
	//		//	handle as variable initialization
	//		//if the keyword is a 
	//	}
	//	break;
	//	}

	//}

	//keep applying operator stack until empty
	return std::make_shared<BaseScriptVariable>();
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

std::shared_ptr<BaseScriptVariable> ScriptBlock::GetVariable(std::string name)
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
