#include "ScriptVariableUtils.h"
#include "ScriptOperator.h"
#include "ScriptError.h"
#include "ScriptVariable.h"
#include <fmt/format.h>
namespace ScriptVariableUtils
{
	template<typename T>
	std::shared_ptr<BaseScriptVariable> ApplyOperation(std::shared_ptr<T>& lhs, std::shared_ptr<T>& rhs, ScriptOperatorType operation);
	template<>
	std::shared_ptr<BaseScriptVariable> ApplyOperation(std::shared_ptr<ScriptBool>& lhs, std::shared_ptr<ScriptBool>& rhs, ScriptOperatorType operation);
	template<>
	std::shared_ptr<BaseScriptVariable> ApplyOperation(std::shared_ptr<ScriptString>& lhs, std::shared_ptr<ScriptString>& rhs, ScriptOperatorType operation);

	template<typename T>
	std::shared_ptr<BaseScriptVariable> ApplyOperation(std::shared_ptr<T>& rhs, ScriptOperatorType operation);
	template<>
	std::shared_ptr<BaseScriptVariable> ApplyOperation(std::shared_ptr<ScriptBool>& rhs, ScriptOperatorType operation);
	template<>
	std::shared_ptr<BaseScriptVariable> ApplyOperation(std::shared_ptr<ScriptString>& rhs, ScriptOperatorType operation);

	template<typename T>
	std::shared_ptr<T> OperatorAssign(std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorAdd(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorSubtract(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorMultiply(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorDivide(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorModulo(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);

	template<typename T>
	std::shared_ptr<ScriptBool> OperatorEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<ScriptBool> OperatorNotEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<ScriptBool> OperatorLarger(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<ScriptBool> OperatorSmaller(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<ScriptBool> OperatorLargerEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<ScriptBool> OperatorSmallerEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);

	template<typename T>
	std::shared_ptr<T> OperatorLogicalAnd(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorLogicalOr(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorBitwiseAnd(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorBitwiseOr(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorBitwiseXor(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs);

	template<typename T>
	std::shared_ptr<T> OperatorSubtract(const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorBitwiseInvert(const std::shared_ptr<T>& rhs);
	template<typename T>
	std::shared_ptr<T> OperatorLogicalNot(const std::shared_ptr<T>& rhs);
}

std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::Operate(std::shared_ptr<BaseScriptVariable>& lhs, std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation)
{
	if(lhs == nullptr || rhs == nullptr)
	{
		throw ScriptError("Attempting to operate on null variables");
	}

	if(lhs->type() != rhs->type())
	{
		throw ScriptError(fmt::format("Attempting to operate on variables with different types. {} {} {}", (int)lhs->type(), (int)operation, (int)rhs->type()));
	}

	if(!lhs->isInitialized() || !rhs->isInitialized())
	{
		throw ScriptError("Attempting to operate on one or more uninitialized variables");
	}

	switch(lhs->type())
	{
	case ScriptVariableType::Bool:
		return ApplyOperation<ScriptBool>(std::static_pointer_cast<ScriptBool>(lhs), std::static_pointer_cast<ScriptBool>(rhs), operation);
	case ScriptVariableType::Dec:
		return ApplyOperation<ScriptDec>(std::static_pointer_cast<ScriptDec>(lhs), std::static_pointer_cast<ScriptDec>(rhs), operation);
	case ScriptVariableType::String:
		return ApplyOperation<ScriptString>(std::static_pointer_cast<ScriptString>(lhs), std::static_pointer_cast<ScriptString>(rhs), operation);
	default:
		throw ScriptError("Operation on variable of undefined type");//add stuff
	}
}

std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::Operate(std::shared_ptr<BaseScriptVariable>& rhs, ScriptOperatorType operation)
{
	if(rhs == nullptr)
	{
		throw ScriptError("Attempting to operate on null variables");
	}

	if(!rhs->isInitialized())
	{
		throw ScriptError("Attempting to operate on an uninitialized variable");
	}

	switch(rhs->type())
	{
	case ScriptVariableType::Bool:
		return ApplyOperation<ScriptBool>(std::static_pointer_cast<ScriptBool>(rhs), operation);
	case ScriptVariableType::Dec:
		return ApplyOperation<ScriptDec>(std::static_pointer_cast<ScriptDec>(rhs), operation);
	case ScriptVariableType::String:
		return ApplyOperation<ScriptString>(std::static_pointer_cast<ScriptString>(rhs), operation);
	default:
		throw ScriptError("Operation on variable of undefined type");//add stuff
	}
}

#pragma region Binary Operator Application

template<typename T>
std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::ApplyOperation(std::shared_ptr<T>& lhs, std::shared_ptr<T>& rhs, ScriptOperatorType operation)
{
	switch(operation)
	{
	case ScriptOperatorType::Assignment:
		return OperatorAssign<T>(lhs, rhs);
	case ScriptOperatorType::Addition:
		return OperatorAdd<T>(lhs, rhs);
	case ScriptOperatorType::Subtraction:
		return OperatorSubtract<T>(lhs, rhs);
	case ScriptOperatorType::Multiplication:
		return OperatorMultiply<T>(lhs, rhs);
	case ScriptOperatorType::Division:
		return OperatorDivide<T>(lhs, rhs);
	case ScriptOperatorType::Modulo:
		return OperatorModulo<T>(lhs, rhs);

	case ScriptOperatorType::Equals:
		return OperatorEquals<T>(lhs, rhs);
	case ScriptOperatorType::NotEquals:
		return OperatorNotEquals<T>(lhs, rhs);
	case ScriptOperatorType::Larger:
		return OperatorLarger<T>(lhs, rhs);
	case ScriptOperatorType::Smaller:
		return OperatorSmaller<T>(lhs, rhs);
	case ScriptOperatorType::LargerEquals:
		return OperatorLargerEquals<T>(lhs, rhs);
	case ScriptOperatorType::SmallerEquals:
		return OperatorSmallerEquals<T>(lhs, rhs);

	case ScriptOperatorType::LogicalAnd:
		return OperatorLogicalAnd<T>(lhs, rhs);
	case ScriptOperatorType::LogicalOr:
		return OperatorLogicalOr<T>(lhs, rhs);

	case ScriptOperatorType::BitwiseAnd:
		return OperatorBitwiseAnd<T>(lhs, rhs);
	case ScriptOperatorType::BitwiseOr:
		return OperatorBitwiseOr<T>(lhs, rhs);
	case ScriptOperatorType::BitwiseXor:
		return OperatorBitwiseXor<T>(lhs, rhs);
	default:
		throw ScriptError(fmt::format("Undefined operator {} {} {}", (int)lhs->type(), (int)operation, (int)rhs->type()));
	}
}

template<>
std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::ApplyOperation(std::shared_ptr<ScriptBool>& lhs, std::shared_ptr<ScriptBool>& rhs, ScriptOperatorType operation)
{
	switch(operation)
	{
	case ScriptOperatorType::Assignment:
		return OperatorAssign<ScriptBool>(lhs, rhs);

	case ScriptOperatorType::Equals:
		return OperatorEquals<ScriptBool>(lhs, rhs);
	case ScriptOperatorType::NotEquals:
		return OperatorNotEquals<ScriptBool>(lhs, rhs);

	case ScriptOperatorType::LogicalAnd:
		return OperatorLogicalAnd<ScriptBool>(lhs, rhs);
	case ScriptOperatorType::LogicalOr:
		return OperatorLogicalOr<ScriptBool>(lhs, rhs);

	default:
		throw ScriptError(fmt::format("Undefined operator {} {} {}", (int)lhs->type(), (int)operation, (int)rhs->type()));
	}
}

template<>
std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::ApplyOperation(std::shared_ptr<ScriptString>& lhs, std::shared_ptr<ScriptString>& rhs, ScriptOperatorType operation)
{
	switch(operation)
	{
	case ScriptOperatorType::Assignment:
		return OperatorAssign<ScriptString>(lhs, rhs);
	case ScriptOperatorType::Addition:
		return OperatorAdd<ScriptString>(lhs, rhs);
	default:
		throw ScriptError(fmt::format("Undefined operator string '{}' string", (int)lhs->type(), (int)operation, (int)rhs->type()));
	}
}

#pragma endregion

#pragma region Unary Operator Application

template<typename T>
std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::ApplyOperation(std::shared_ptr<T>& rhs, ScriptOperatorType operation)
{
	switch(operation)
	{
	case ScriptOperatorType::Subtraction:
		return OperatorSubtract<T>(rhs);
	case ScriptOperatorType::LogicalNot:
		return OperatorLogicalNot<T>(rhs);
	case ScriptOperatorType::BitwiseInvert:
		return OperatorBitwiseInvert<T>(rhs);
	default:
		throw ScriptError(fmt::format("Undefined operator {} {}", (int)operation, (int)rhs->type()));
	}
}

template<>
std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::ApplyOperation(std::shared_ptr<ScriptBool>& rhs, ScriptOperatorType operation)
{
	switch(operation)
	{
	case ScriptOperatorType::LogicalNot:
		return OperatorLogicalNot<ScriptBool>(rhs);
	default:
		throw ScriptError(fmt::format("Undefined operator {} {}", (int)operation, (int)rhs->type()));
	}
}

template<>
std::shared_ptr<BaseScriptVariable> ScriptVariableUtils::ApplyOperation(std::shared_ptr<ScriptString>& rhs, ScriptOperatorType operation)
{
	throw ScriptError(fmt::format("Undefined operator {} {}", (int)operation, (int)rhs->type()));
}

#pragma endregion

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorAssign(std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	lhs->assign(*rhs);
	return lhs;
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorAdd(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() + rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorSubtract(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() - rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorMultiply(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() * rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorDivide(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() / rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorModulo(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() % rhs->value());
}

template<typename T>
std::shared_ptr<ScriptBool> ScriptVariableUtils::OperatorEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<ScriptBool>(lhs->value() == rhs->value());
}

template<typename T>
std::shared_ptr<ScriptBool> ScriptVariableUtils::OperatorNotEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<ScriptBool>(lhs->value() != rhs->value());
}

template<typename T>
std::shared_ptr<ScriptBool> ScriptVariableUtils::OperatorLarger(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<ScriptBool>(lhs->value() > rhs->value());
}

template<typename T>
std::shared_ptr<ScriptBool> ScriptVariableUtils::OperatorSmaller(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<ScriptBool>(lhs->value() < rhs->value());
}

template<typename T>
std::shared_ptr<ScriptBool> ScriptVariableUtils::OperatorLargerEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<ScriptBool>(lhs->value() >= rhs->value());
}

template<typename T>
std::shared_ptr<ScriptBool> ScriptVariableUtils::OperatorSmallerEquals(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<ScriptBool>(lhs->value() <= rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorLogicalAnd(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() && rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorLogicalOr(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() || rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorBitwiseAnd(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() & rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorBitwiseOr(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() | rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorBitwiseXor(const std::shared_ptr<T>& lhs, const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(lhs->value() ^ rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorSubtract(const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(-rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorLogicalNot(const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(!rhs->value());
}

template<typename T>
std::shared_ptr<T> ScriptVariableUtils::OperatorBitwiseInvert(const std::shared_ptr<T>& rhs)
{
	return std::make_shared<T>(~rhs->value());
}