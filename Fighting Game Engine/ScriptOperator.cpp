#include "ScriptOperator.h"

const std::map<std::string, ScriptOperatorType> ScriptOperator::operatorLookup = {
	{ "=", ScriptOperatorType::Assignment },

	{ "+", ScriptOperatorType::Addition },
	{ "-", ScriptOperatorType::Subtraction },
	{ "*", ScriptOperatorType::Multiplication },
	{ "/", ScriptOperatorType::Division },
	{ "%", ScriptOperatorType::Modulo },

	{ "==", ScriptOperatorType::Equals },
	{ "!=", ScriptOperatorType::NotEquals },
	{ ">", ScriptOperatorType::Larger },
	{ "<", ScriptOperatorType::Smaller },
	{ ">=", ScriptOperatorType::LargerEquals },
	{ "<=", ScriptOperatorType::SmallerEquals },

	{ "&&", ScriptOperatorType::LogicalAnd },
	{ "||", ScriptOperatorType::LogicalOr },
	{ "!", ScriptOperatorType::LogicalNot },

	{ "&", ScriptOperatorType::BitwiseAnd },
	{ "|", ScriptOperatorType::BitwiseOr },
	{ "^", ScriptOperatorType::BitwiseXor },
	{ "~", ScriptOperatorType::BitwiseInvert },
};