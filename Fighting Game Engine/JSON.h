#pragma once
#include <nlohmann/json.hpp>
#include "ValEngine.h"
#include "MathIncludes.hpp"
#include "TemplateUtils.h"
#include "DebugLog.h"

class IReflectable;

namespace JSON
{
	using json_t = nlohmann::json;

	template<typename T>
	bool TryGetMember(const json_t& j, const std::string& name, T& out_result);
	bool HasMember(const json_t& j, const std::string& name);

	template<typename T>
	T Get(const json_t& j);

	template<>
	FixedPoint64 Get(const json_t& j);
	template<>
	unsigned char Get(const json_t& j);

	template<>
	ve::vec2 Get(const json_t& j);
	template<>
	ve::ivec2 Get(const json_t& j);
	template<>
	ve::vec3 Get(const json_t& j);
	template<>
	ve::ivec3 Get(const json_t& j);
	template<>
	ve::vec4 Get(const json_t& j);
	template<>
	ve::ivec4 Get(const json_t& j);

	template<>
	glm::quat Get(const json_t& j);

	template<typename ValueT>
	json_t ToJson(const ValueT& value);
}

using json = JSON::json_t;

template<typename T>
T JSON::Get(const json_t & j)
{
	if constexpr(std::is_constructible_v<T, const json&>)
	{
		return T(j);
	}
	else if constexpr(std::is_base_of_v<IReflectable, T>)
	{
		T val;
		val.Deserialize(j);
		return val;
	}
	else
	{
		return j.get<T>();
	}
}

template<typename T>
bool JSON::TryGetMember(const json_t & j, const std::string& name, T& out_result)
{
	auto& iter = j.find(name);
	if(iter == j.end())
	{
		return false;
	}

	out_result = JSON::Get<T>(iter.value());
	return true;
}

template <typename ValueT>
JSON::json_t JSON::ToJson(const ValueT& value)
{
	if constexpr(std::is_base_of_v<IReflectable, ValueT>)
	{
		return value.Serialize();
	}
	else
	{
		return json_t(value);
	}
}