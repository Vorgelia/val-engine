#pragma once
#include <memory>

class BaseScriptVariable;
enum class ScriptOperatorType;

namespace ScriptVariableUtils
{
	std::shared_ptr<BaseScriptVariable> Operate(std::shared_ptr<BaseScriptVariable>& lhs, std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation);
	std::shared_ptr<BaseScriptVariable> Operate(std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation);
}