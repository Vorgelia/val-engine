#pragma once
#include <map>
#include <string>

enum class ScriptOperatorType
{
	Assignment,

	Addition,
	Subtraction,
	Multiplication,
	Division,
	Modulo,

	Equals,
	NotEquals,
	Larger,
	Smaller,
	LargerEquals,
	SmallerEquals,

	LogicalAnd,
	LogicalOr,
	LogicalNot,

	BitwiseAnd,
	BitwiseOr,
	BitwiseXor,
	BitwiseInvert
};

class ScriptOperator
{
public:
	static const std::map<std::string, ScriptOperatorType> operatorLookup;
};