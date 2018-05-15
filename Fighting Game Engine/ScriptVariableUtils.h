#pragma once
#include <memory>
#include "JSON.h"

class BaseScriptVariable;
enum class ScriptOperatorType;

namespace ScriptVariableUtils
{
	json ToJson(std::shared_ptr<BaseScriptVariable> var);
	std::shared_ptr<BaseScriptVariable> FromJson(const json& j);

	std::shared_ptr<BaseScriptVariable> Operate(std::shared_ptr<BaseScriptVariable>& lhs, std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation);
	std::shared_ptr<BaseScriptVariable> Operate(std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation);
}