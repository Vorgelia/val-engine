#include "ScriptBlock.h"
#include "Script.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"

size_t ScriptBlock::cursor()
{
	return _cursor;
}

//TODO: Make parse line return a token vector
void ScriptBlock::ParseLine(const std::string &line)
{
	if(line[0] == '#')
	{
		return;
	}

	int cursor = 0;
	int endIndex;

	std::vector<ScriptToken> tokens;
	ScriptParsingUtils::ParseLineTokens(line, tokens);

	if(tokens.size == 0)
	{
		return;
	}

	if(tokens[0].token == "function")
	{
		HandleFunctionDeclarationLine(tokens);
	}
	else if(tokens[0].token == "if")
	{
		HandleConditionalDeclarationLine(tokens);
	}
	else if(tokens[0].token == "while")
	{
		HandleLoopDeclarationLine(tokens);
	}
	else
	{
		HandleExpressionLine(tokens);
	}
}

void ScriptBlock::HandleExpressionLine(const std::vector<ScriptToken> &tokens)
{
	if(tokens[0].token == "return")
	{
		_owner->RaiseControlFlag(ScriptControlFlag::Return);
	}
	else if(tokens[0].token == "break")
	{
		_owner->RaiseControlFlag(ScriptControlFlag::Break);
		return;
	}
	else if(tokens[0].token == "continue")
	{
		_owner->RaiseControlFlag(ScriptControlFlag::Continue);
		return;
	}
}

void ScriptBlock::HandleFunctionDeclarationLine(const std::vector<ScriptToken> &tokens)
{
	throw ScriptError("Misplaced function declaration.");
}

void ScriptBlock::HandleLoopDeclarationLine(const std::vector<ScriptToken> &tokens)
{

}

void ScriptBlock::HandleConditionalDeclarationLine(const std::vector<ScriptToken> &tokens)
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
		else if(depth < _depth)
		{
			throw ScriptError("Parser error: Line of invalid depth within block.");
		}

		ParseLine(line);
		//TODO: Proper parsing

		/* ... */

		if(HandleControlFlag())
		{
			break;
		}
	}
	//run normally
	//if parent isn't null, throw exceptions on class and function declarations
	//otherwise skip to end of those blocks and continue
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
