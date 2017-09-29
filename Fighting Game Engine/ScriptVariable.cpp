#include "ScriptVariable.h"

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