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
private:
	static const std::map<std::string, ScriptOperator> _operatorLookup;

	ScriptOperatorType _type;
	int _priority;

public:
	ScriptOperatorType type() const;
	int priority() const;

	static const ScriptOperator* GetOperator(const std::string& signature);

	ScriptOperator(ScriptOperatorType type, int priority);
};