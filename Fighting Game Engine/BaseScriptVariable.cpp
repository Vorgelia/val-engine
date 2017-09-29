#include "BaseScriptVariable.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptOperator.h"

ScriptVariableType BaseScriptVariable::type() const
{
	return _type;
}

bool BaseScriptVariable::isConst() const
{
	return _const;
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