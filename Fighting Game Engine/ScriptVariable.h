#pragma once
#include <memory>
#include <string>
#include "BaseScriptVariable.h"

template<typename T>
class ScriptVariable :
	public BaseScriptVariable
{
	T _value;
public:
	T value() const;
	std::string ToString() override;

	void assign(const ScriptVariable<T>& value);
	ScriptVariable(T value = T(), bool isConst = false);
};

typedef ScriptVariable<long> ScriptInt;
typedef ScriptVariable<bool> ScriptBool;
typedef ScriptVariable<std::string> ScriptString;

template<typename T>
inline T ScriptVariable<T>::value() const
{
	return _value;
}

template<typename T>
std::string ScriptVariable<T>::ToString()
{
	return std::to_string(_value);
}

template<typename T>
inline void ScriptVariable<T>::assign(const ScriptVariable<T>& value)
{
	if(_const)
	{
		throw ScriptError("Attempting to modify const variable.");
	}
	_value = value.value();
}

template<typename T>
inline ScriptVariable<T>::ScriptVariable(T value, bool isConst)
{
	_value = value;
	_initialized = true;
}

template<>
std::string ScriptInt::ToString();
template<>
std::string ScriptBool::ToString();
template<>
std::string ScriptString::ToString();

template<>
ScriptInt::ScriptVariable(long value, bool isConst);
template<>
ScriptBool::ScriptVariable(bool value, bool isConst);
template<>
ScriptString::ScriptVariable(std::string value, bool isConst);