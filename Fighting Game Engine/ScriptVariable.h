#pragma once
#include <string>
#include <cstdint>
#include "ValEngine.h"
#include "BaseScriptVariable.h"
#include "ScriptError.h"

template<typename T, ScriptVariableType VariableType>
class ScriptVariable : public BaseTypedScriptVariable<VariableType>
{
public:
	typedef T value_type;

protected:
	T _value;

public:
	T value() const;

	std::string ToString() const override; 
	json ToJSON() const override;
	std::shared_ptr<BaseScriptVariable> Clone() const override;

	void assign(const ScriptVariable<T, VariableType>& value);

	ScriptVariable(const ScriptVariable<T, VariableType>& other);
	explicit ScriptVariable(const json& j);
	ScriptVariable(T value = T(), bool isConst = false);
};

typedef ScriptVariable<ve::dec_t, ScriptVariableType::Dec> ScriptDec;
typedef ScriptVariable<bool, ScriptVariableType::Bool> ScriptBool;
typedef ScriptVariable<std::string, ScriptVariableType::String> ScriptString;

template<typename T, ScriptVariableType VariableType>
T ScriptVariable<T, VariableType>::value() const
{
	return _value;
}

template<typename T, ScriptVariableType VariableType>
std::string ScriptVariable<T, VariableType>::ToString() const
{
	return std::to_string(_value);
}

template<typename T, ScriptVariableType VariableType>
inline json ScriptVariable<T, VariableType>::ToJSON() const
{
	json parentJson = BaseTypedScriptVariable<VariableType>::ToJSON();
	parentJson.emplace("value", _value);
	return parentJson;
}

template <typename T, ScriptVariableType VariableType>
std::shared_ptr<BaseScriptVariable> ScriptVariable<T, VariableType>::Clone() const
{
	return std::make_shared<ScriptVariable<T, VariableType>>(*this);
}

template<typename T, ScriptVariableType VariableType>
void ScriptVariable<T, VariableType>::assign(const ScriptVariable<T, VariableType>& value)
{
	if(_const)
	{
		throw ScriptError("Attempting to modify const variable.");
	}
	_value = value.value();
	_initialized = true;
}

template <typename T, ScriptVariableType VariableType>
ScriptVariable<T, VariableType>::ScriptVariable(const json& j)
	: BaseTypedScriptVariable<VariableType>(j)
{
	if(JSON::TryGetMember(j, "value", _value))
	{
		_initialized = true;
	}
}

template<typename T, ScriptVariableType VariableType>
ScriptVariable<T, VariableType>::ScriptVariable(T value, bool isConst)
	: BaseTypedScriptVariable<VariableType>(isConst)
	, _value(value)
{
	_initialized = true;
}

template <typename T, ScriptVariableType VariableType>
ScriptVariable<T, VariableType>::ScriptVariable(const ScriptVariable<T, VariableType>& other)
	: _value(other.value())
{
	_const = false;
	_initialized = true;
}

template<>
std::string ScriptString::ToString() const;

template<>
std::string ScriptDec::ToString() const;
template<>
json ScriptDec::ToJSON() const;
template<>
ScriptDec::ScriptVariable(const json& j);