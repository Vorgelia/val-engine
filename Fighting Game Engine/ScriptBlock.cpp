#include "ScriptBlock.h"
#include "Script.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"

//TODO: Make parse line return a token vector
void ScriptBlock::ParseLine(const std::string &line, int lineIndex)
{
	if(line[0] == '#')
	{
		return;
	}

	int cursor = 0;
	int endIndex;

	ScriptTokenType type = ScriptParsingUtils::GetNextTokenType(line, cursor, endIndex);
	if(endIndex < 0 || type == ScriptTokenType::Invalid)
	{
		throw ScriptError("Unexpected parsing error with token " + std::to_string((int)type), _owner->name(), _lines.front() + lineIndex);
	}

	std::string token = line.substr(cursor, endIndex - cursor + 1);
	if(token == "return")
	{
		_owner->RaiseControlFlag(ScriptControlFlag::Return);
	}
}

void ScriptBlock::Run()
{
	for(size_t i = 0; i < _lines.size(); ++i)
	{
		int depth;
		std::string line = ScriptParsingUtils::TrimLine(_lines[i], depth);

		if(depth < 0)
		{
			continue;
		}
		else if(depth < _depth)
		{
			throw ScriptError("Parser error: Line of invalid depth within block.", _owner->name(), _lines.front() + i);
		}

		ParseLine(line, i);
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
