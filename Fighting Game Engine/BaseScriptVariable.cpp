#include "BaseScriptVariable.h"
#include "ScriptParsingUtils.h"
#include "ScriptError.h"
#include "ScriptOperator.h"
#include "ScriptToken.h"

const std::map<std::string, ScriptVariableType> BaseScriptVariable::variableTypeLookup =
{
	{ ScriptToken::type_int, ScriptVariableType::Int },
	{ ScriptToken::type_bool, ScriptVariableType::Bool },
	{ ScriptToken::type_string, ScriptVariableType::String },
};

ScriptVariableType BaseScriptVariable::GetVariableType(const std::string & token)
{
	auto& iter = variableTypeLookup.find(token);
	if(iter == variableTypeLookup.end())
	{
		return ScriptVariableType::Null;
	}

	return iter->second;
}

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
	_initialized = false;
}

BaseScriptVariable::BaseScriptVariable()
{
	_type = ScriptVariableType::Null;
	_const = false;
	_initialized = false;
}

BaseScriptVariable::~BaseScriptVariable()
{
}