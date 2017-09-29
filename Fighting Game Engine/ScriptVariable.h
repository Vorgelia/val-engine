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
	void assign(const ScriptVariable<T>& value);
	ScriptVariable(T value = T(), bool isConst = false);
};

typedef ScriptVariable<int> ScriptInt;
typedef ScriptVariable<bool> ScriptBool;
typedef ScriptVariable<std::string> ScriptString;

template<typename T>
inline T ScriptVariable<T>::value() const
{
	return _value;
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
}

template<>
ScriptInt::ScriptVariable(int value, bool isConst);
template<>
ScriptBool::ScriptVariable(bool value, bool isConst);
template<>
ScriptString::ScriptVariable(std::string value, bool isConst);