#pragma once
#include <json.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>
#include "DebugLog.h"

#define VE_JSON_T(type)\
	typedef type json_t

namespace JSON
{
	VE_JSON_T(nlohmann::json);
	template<typename T>
	T Get(const json_t& j);

	template<>
	glm::quat Get(const json_t& j);
	template<>
	glm::ivec2 Get(const json_t& j);
	template<>
	glm::vec2 Get(const json_t& j);
}

using json = JSON::json_t;

#undef VE_JSON_T

template<typename T>
T JSON::Get(const json & j)
{
	return j.get<T>();
}