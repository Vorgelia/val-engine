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

template<>
json ScriptDec::ToJSON() const
{
	//TODO: oh no
	return json(_value.bits());
}

template<>
ScriptDec::ScriptVariable(const json& j)
{
	_value = value_type::FromRawBits(j.get<std::int64_t>());
}