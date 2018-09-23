#include "ScriptVariable.h"

template<>
std::string ScriptString::ToString() const
{
	return _value;
}

template<>
std::string ScriptDec::ToString() const
{
	return std::to_string(double(_value));
}