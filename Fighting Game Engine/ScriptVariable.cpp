#include "ScriptVariable.h"

ScriptVariable::ScriptVariable(std::string type, std::string value)
{
	_type = type;
	_value = value;
}

ScriptVariable::ScriptVariable()
{
	_type = "null";
	_value = "null";
}

ScriptVariable::~ScriptVariable()
{
}