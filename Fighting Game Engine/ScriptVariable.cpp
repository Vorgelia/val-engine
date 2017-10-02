#include "ScriptVariable.h"

template<>
std::string ScriptInt::ToString()
{
	return std::to_string(_value);
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
ScriptInt::ScriptVariable(int value, bool isConst) : BaseScriptVariable(ScriptVariableType::Int, isConst)
{

}

template<>
ScriptBool::ScriptVariable(bool value, bool isConst) : BaseScriptVariable(ScriptVariableType::Bool, isConst)
{

}

template<>
ScriptString::ScriptVariable(std::string value, bool isConst) : BaseScriptVariable(ScriptVariableType::String, isConst)
{

}