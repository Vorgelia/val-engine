#include "BaseScriptVariable.h"
#include "ScriptError.h"
#include "ScriptOperator.h"
#include "ScriptToken.h"

const std::unordered_map<std::string, ScriptVariableType> BaseScriptVariable::variableTypeLookup =
{
	{ ScriptToken::type_int, ScriptVariableType::Dec },
	{ ScriptToken::type_bool, ScriptVariableType::Bool },
	{ ScriptToken::type_string, ScriptVariableType::String },
	{ ScriptToken::type_collection, ScriptVariableType::Collection },
	{ ScriptToken::type_void, ScriptVariableType::Null },
};

ScriptVariableType BaseScriptVariable::GetVariableType(const std::string & token)
{
	auto& iter = variableTypeLookup.find(token);
	if(iter == variableTypeLookup.end())
	{
		return ScriptVariableType::Invalid;
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

bool BaseScriptVariable::isInitialized() const
{
	return _initialized;
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
= default;
