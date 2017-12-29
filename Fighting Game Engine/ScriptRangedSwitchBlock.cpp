#include "ScriptRangedSwitchBlock.h"
#include "ScriptParsingUtils.h"
#include "ScriptVariableUtils.h"
#include "ScriptExpression.h"
#include "ScriptOperator.h"
#include "ScriptError.h"
#include "Script.h"

#include <boost\lexical_cast.hpp>

bool ScriptRangedSwitchBlock::HandleControlFlag()
{
	switch(_owner->controlFlag())
	{
	case ScriptControlFlag::Break:
		_owner->ConsumeControlFlag();
	case ScriptControlFlag::Continue:
	case ScriptControlFlag::Return:
		return true;
	}
	return false;
}

void ScriptRangedSwitchBlock::Run()
{
	for(_cursor = 0; _cursor < _lines.size(); ++_cursor)
	{
		if(_handledCaseLabel)
		{
			if(_lines[_cursor].depth < 0)
			{
				continue;
			}
			else if(_lines[_cursor].depth != _depth)
			{
				throw ScriptError("Parser error: Line of invalid depth within block.");
			}
		}

		ParseLine(_lines[_cursor]);

		if(HandleControlFlag())
		{
			break;
		}
	}
}

void ScriptRangedSwitchBlock::ParseLine(ScriptLine & line)
{
	if(line.tokens.size() > 1)
	{
		if(line.tokens[0].token == ScriptToken::switch_case)
		{
			if(line.depth != _depth)
			{
				throw ScriptError("Parser error: Line of invalid depth within block.");
			}

			if(_handledCaseLabel
				|| (_handledCaseLabel = (line.tokens[1].token == ScriptToken::switch_default && line.tokens.size() == 2)))
			{
				return;
			}

			std::shared_ptr<BaseScriptVariable> lowerBound, upperBound;

			switch(line.tokens.size())
			{
			case 2:
				lowerBound = ScriptExpression(this, std::vector<ScriptToken>{line.tokens[1]}).Evaluate();
				_handledCaseLabel = _conditionResult->value() == std::static_pointer_cast<ScriptInt>(lowerBound)->value();
				return;
			case 4:
				if(line.tokens[2].token != "->")
				{
					break;
				}

				lowerBound = ScriptExpression(this, std::vector<ScriptToken>{line.tokens[1]}).Evaluate();
				upperBound = ScriptExpression(this, std::vector<ScriptToken>{line.tokens[3]}).Evaluate();

				if(lowerBound->type() != ScriptVariableType::Int || upperBound->type() != ScriptVariableType::Int)
				{
					break;
				}

				_handledCaseLabel =
					std::static_pointer_cast<ScriptBool>(ScriptVariableUtils::Operate(std::static_pointer_cast<BaseScriptVariable>(_conditionResult), lowerBound, ScriptOperatorType::LargerEquals))->value()
					&& std::static_pointer_cast<ScriptBool>(ScriptVariableUtils::Operate(std::static_pointer_cast<BaseScriptVariable>(_conditionResult), upperBound, ScriptOperatorType::SmallerEquals))->value();
				return;
			}

			throw ScriptError("Invalid tokens in case label.");
		}
	}

	if(_handledCaseLabel)
	{
		ScriptBlock::ParseLine(line);
	}
}

bool ScriptRangedSwitchBlock::HandleConditionResult(std::shared_ptr<BaseScriptVariable> result)
{
	if(result->type() != ScriptVariableType::Int)
	{
		throw ScriptError("Invalid result type in ranged switch expression.");
	}

	_conditionResult = std::static_pointer_cast<ScriptInt>(result);

	Run();
	return true;
}

ScriptRangedSwitchBlock::ScriptRangedSwitchBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock * parent, Script * owner)
	:ScriptConditionalBlock(conditionTokens, lines, depth, parent, owner)
{
	_handledCaseLabel = false;
}

ScriptRangedSwitchBlock::~ScriptRangedSwitchBlock()
{
}
