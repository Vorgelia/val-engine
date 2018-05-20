#pragma once
#include <nlohmann/json.hpp>
#include "ValEngine.h"
#include "MathIncludes.hpp"
#include "TemplateUtils.h"
#include "DebugLog.h"
#include "CollisionBox.h"

class IReflectable;

//TODO: Cleanup all of this with if constexpr
namespace JSON
{
	using json_t = nlohmann::json;

	struct json_constructible_tag {};
	struct json_reflectable_tag {};
	struct json_serializable_tag {};

	template<typename T>
	bool TryGetMember(const json_t& j, const std::string& name, T& out_result);

	template<typename T>
	T Get(const json_t& j);
	template<typename T>
	T Get(const json_t& j, json_constructible_tag);
	template<typename T>
	T Get(const json_t& j, json_serializable_tag);

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
	glm::vec2 Get(const json_t& j);
	template<>
	glm::ivec2 Get(const json_t& j);
	template<>
	glm::vec4 Get(const json_t& j);
	template<>
	glm::ivec4 Get(const json_t& j);

	template<>
	glm::quat Get(const json_t& j);
	template<>
	CollisionBox Get(const json_t& j);

	template<typename ValueT>
	json_t ToJson(const ValueT& value);
	template<typename ValueT>
	json_t ToJson(const ValueT& value, json_reflectable_tag);
	template<typename ValueT>
	json_t ToJson(const ValueT& value, json_serializable_tag);
}

using json = JSON::json_t;

template<typename T>
T JSON::Get(const json_t & j)
{
	return JSON::Get<T>(j, std::conditional_t<std::is_constructible_v<T, const json&>, json_constructible_tag, json_serializable_tag> {});
}

template <typename T>
T JSON::Get(const json_t& j, json_constructible_tag)
{
	return T(j);
}

template <typename T>
T JSON::Get(const json_t& j, json_serializable_tag)
{
	return j.get<T>();
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
	return JSON::ToJson<ValueT>(value, std::conditional_t<std::is_base_of_v<IReflectable, ValueT>, json_reflectable_tag, json_serializable_tag> {});
}

template <typename ValueT>
JSON::json_t JSON::ToJson(const ValueT& value, json_reflectable_tag)
{
	return value.Serialize();
}

template <typename ValueT>
JSON::json_t JSON::ToJson(const ValueT& value, json_serializable_tag)
{
	return json_t(value);
}