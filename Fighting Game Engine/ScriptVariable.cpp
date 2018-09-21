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
	json parentJson = Super::ToJSON();
	parentJson.emplace("ve_value", _value.bits());
	return parentJson;
}

template<>
ScriptDec::ScriptVariable(const json& j)
	: Super(j)
{
	if(!ScriptVariableUtils::JsonIsScriptVariableObject(j))
	{

		_value = JSON::Get<value_type>(j);
		_initialized = true;
	}
	else
	{
		std::int64_t rawBits = 0;
		if(JSON::TryGetMember(j, "ve_value", rawBits))
		{
			_value = value_type::FromRawBits(rawBits);
			_initialized = true;
		}
	}
}