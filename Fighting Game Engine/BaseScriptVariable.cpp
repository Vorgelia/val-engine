#include "BaseScriptVariable.h"
#include "ScriptError.h"
#include "ScriptOperator.h"
#include "ScriptToken.h"
#include "IReflectable.h"
#include "SerializationProxy.h"

const std::unordered_map<std::string, ScriptVariableType> BaseScriptVariable::variableTypeLookup =
{
	{ ScriptToken::type_dec, ScriptVariableType::Dec },
	{ ScriptToken::type_bool, ScriptVariableType::Bool },
	{ ScriptToken::type_string, ScriptVariableType::String },
	{ ScriptToken::type_map, ScriptVariableType::Map },
	{ ScriptToken::type_array, ScriptVariableType::Array },
	{ ScriptToken::type_void, ScriptVariableType::Null },
};

ScriptVariableType BaseScriptVariable::GetVariableTypeFromToken(const std::string & token)
{
	auto& iter = variableTypeLookup.find(token);
	if(iter == variableTypeLookup.end())
	{
		return ScriptVariableType::Invalid;
	}

	return iter->second;
}

bool BaseScriptVariable::isConst() const
{
	return _const;
}

bool BaseScriptVariable::isInitialized() const
{
	return _initialized;
}

std::string BaseScriptVariable::ToString() const
{
	return ScriptToken::value_null;
}

json BaseScriptVariable::ToJSON() const
{
	return json
	{
		{ "ve_type", int(type()) },
		{ "ve_const", _const },
		{ "ve_initialized", _initialized },
	};
}

std::shared_ptr<BaseScriptVariable> BaseScriptVariable::Clone() const
{
	return std::make_shared<BaseScriptVariable>(false);
}

BaseScriptVariable::BaseScriptVariable(bool isConst)
	: _const(isConst)
	, _initialized(false)
{
}

BaseScriptVariable::BaseScriptVariable(const json & j)
{
	JSON::TryGetMember(j, "ve_const", _const);
	JSON::TryGetMember(j, "ve_initialized", _initialized);
}

BaseScriptVariable::BaseScriptVariable(const IReflectable& reflectable)
{
	json j;
	JsonSerializationProxy proxy{ j };
	reflectable.SerializeProxy(proxy);
	proxy.Get("ve_const", _const);
	proxy.Get("ve_initialized", _initialized);
}
