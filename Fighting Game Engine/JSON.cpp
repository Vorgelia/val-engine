#include "JSON.h"

template<>
glm::quat JSON::Get(const json_t& j)
{
	return glm::quat(
		j["w"].get<float>(),
		j["x"].get<float>(),
		j["y"].get<float>(),
		j["z"].get<float>()
	);
}

template<>
glm::ivec2 JSON::Get(const json_t& j)
{
	return glm::ivec2(
		j["x"].get<int>(),
		j["y"].get<int>()
	);
}

template<>
glm::vec2 JSON::Get(const json_t& j)
{
	return glm::vec2(
		j["x"].get<float>(),
		j["y"].get<float>()
	);
}