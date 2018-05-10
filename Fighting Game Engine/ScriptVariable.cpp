#include "ScriptVariable.h"
#include <utility>

template<>
std::string ScriptDec::ToString()
{
	return std::to_string(double(_value));
}

template<>
std::string ScriptBool::ToString()
{
	return std::to_string(_value);
}

template<>
std::string ScriptString::ToString()
{
	return _value;
}

template<>
ScriptDec::ScriptVariable(ve::dec_t value, bool isConst) : BaseScriptVariable(ScriptVariableType::Dec, isConst)
{
	_value = value;
	_initialized = true;
}

template<>
ScriptBool::ScriptVariable(bool value, bool isConst) : BaseScriptVariable(ScriptVariableType::Bool, isConst)
{
	_value = value;
	_initialized = true;
}

template<>
ScriptString::ScriptVariable(std::string value, bool isConst) : BaseScriptVariable(ScriptVariableType::String, isConst)
{
	_value = std::move(value);
	_initialized = true;
}