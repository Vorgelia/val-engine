#pragma once
#include <json.hpp>
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>
#include "DebugLog.h"

namespace JSON
{
	typedef nlohmann::json json_t;

	template<typename T>
	bool TryGetMember(const json_t& j, std::string name, T& out_result);

	template<typename T>
	T Get(const json_t& j);
	template<>
	glm::quat Get(const json_t& j);
	template<>
	glm::ivec4 Get(const json_t& j);
	template<>
	glm::vec4 Get(const json_t& j);
	template<>
	glm::ivec2 Get(const json_t& j);
	template<>
	glm::vec2 Get(const json_t& j);
	template<>
	unsigned char Get(const json_t& j);
}

using json = JSON::json_t;

template<typename T>
T JSON::Get(const json & j)
{
	return j.get<T>();
}

template<typename T>
bool JSON::TryGetMember(const json_t & j, std::string name, T& out_result)
{
	auto& iter = j.find(name);
	if(iter == j.end())
	{
		return false;
	}

	out_result = JSON::Get<T>(iter.value());
	return true;
}