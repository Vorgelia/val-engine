#pragma once
#include <memory>
#include "JSON.h"
#include "BaseScriptVariable.h"

class BaseScriptVariable;
enum class ScriptOperatorType;

namespace ScriptVariableUtils
{
	json ToJson(std::shared_ptr<BaseScriptVariable> var);
	std::shared_ptr<BaseScriptVariable> FromJson(const json& j);

	template<typename T>
	std::shared_ptr<T> Cast(const std::shared_ptr<BaseScriptVariable>& var);

	bool JsonIsScriptVariableObject(const json& j);

	std::shared_ptr<BaseScriptVariable> Operate(std::shared_ptr<BaseScriptVariable>& lhs, std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation);
	std::shared_ptr<BaseScriptVariable> Operate(std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation);
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::Cast(const std::shared_ptr<BaseScriptVariable>& var)
{
	if(T::variable_type != var->type())
	{
		return nullptr;
	}
	return std::static_pointer_cast<T>(var);
}