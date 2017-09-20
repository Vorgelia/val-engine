#include "ScriptVariable.h"
#include "ScriptParsingUtils.h"

ScriptVariable::ScriptVariable(std::string type)
{
	_type = type;
	_value = ScriptToken::null_token;
}

ScriptVariable::ScriptVariable(std::string type, std::string value)
{
	_type = type;
	_value = value;
}

ScriptVariable::ScriptVariable()
{
	_type = ScriptToken::null_token;
	_value = ScriptToken::null_token;
}

ScriptVariable::~ScriptVariable()
{
}