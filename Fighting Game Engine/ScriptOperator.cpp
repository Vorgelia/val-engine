#include "ScriptOperator.h"

const std::unordered_map<std::string, ScriptOperator> ScriptOperator::_operatorLookup = {
	{ "=", ScriptOperator(ScriptOperatorType::Assignment, 15) },

	{ "+", ScriptOperator(ScriptOperatorType::Addition, 6) },
	{ "-", ScriptOperator(ScriptOperatorType::Subtraction, 6) },
	{ "*", ScriptOperator(ScriptOperatorType::Multiplication, 5) },
	{ "/", ScriptOperator(ScriptOperatorType::Division, 5) },
	{ "%", ScriptOperator(ScriptOperatorType::Modulo, 5) },

	{ "==", ScriptOperator(ScriptOperatorType::Equals, 9) },
	{ "!=", ScriptOperator(ScriptOperatorType::NotEquals, 9) },
	{ ">", ScriptOperator(ScriptOperatorType::Larger, 8) },
	{ "<", ScriptOperator(ScriptOperatorType::Smaller, 8) },
	{ ">=", ScriptOperator(ScriptOperatorType::LargerEquals, 8) },
	{ "<=", ScriptOperator(ScriptOperatorType::SmallerEquals, 8) },

	{ "&&", ScriptOperator(ScriptOperatorType::LogicalAnd, 13) },
	{ "||", ScriptOperator(ScriptOperatorType::LogicalOr, 14) },
	{ "!", ScriptOperator(ScriptOperatorType::LogicalNot, 3) },

	{ "&", ScriptOperator(ScriptOperatorType::BitwiseAnd, 10) },
	{ "|", ScriptOperator(ScriptOperatorType::BitwiseOr, 12) },
	{ "^", ScriptOperator(ScriptOperatorType::BitwiseXor, 11) },
	{ "~", ScriptOperator(ScriptOperatorType::BitwiseInvert, 3) },
};

ScriptOperatorType ScriptOperator::type() const
{
	return _type;
}

int ScriptOperator::priority() const
{
	return _priority;
}

const ScriptOperator * ScriptOperator::GetOperator(const std::string& signature)
{
	auto iter = _operatorLookup.find(signature);
	if(iter == _operatorLookup.end())
	{
		return nullptr;
	}

	return &(iter->second);
}

ScriptOperator::ScriptOperator(ScriptOperatorType type, int priority)
{
	_type = type;
	_priority = priority;
}
