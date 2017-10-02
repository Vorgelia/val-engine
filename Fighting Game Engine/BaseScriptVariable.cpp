#include "BaseScriptVariable.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptOperator.h"
#include "ScriptToken.h"

ScriptVariableType BaseScriptVariable::type() const
{
	return _type;
}

bool BaseScriptVariable::isConst() const
{
	return _const;
}

std::string BaseScriptVariable::ToString()
{
	return ScriptToken::value_null;
}

BaseScriptVariable::BaseScriptVariable(ScriptVariableType type, bool isConst)
{
	_type = type;
	_const = isConst;
	_initialized = true;
}

BaseScriptVariable::BaseScriptVariable()
{
	_type = ScriptVariableType::Error;
	_const = false;
	_initialized = false;
}

BaseScriptVariable::~BaseScriptVariable()
{
}