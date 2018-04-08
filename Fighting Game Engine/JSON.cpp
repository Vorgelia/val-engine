#include "JSON.h"
#include "GameCharacterData.h"

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
glm::ivec4 JSON::Get(const json_t& j)
{
	return glm::ivec4(
		j["x"].get<int>(),
		j["y"].get<int>(),
		j["z"].get<int>(),
		j["w"].get<int>()
	);
}

template<>
glm::lvec4 JSON::Get(const json_t& j)
{
	return glm::lvec4(
		j["x"].get<std::int64_t>(),
		j["y"].get<std::int64_t>(),
		j["z"].get<std::int64_t>(),
		j["w"].get<std::int64_t>()
	);
}

template<>
glm::vec4 JSON::Get(const json_t& j)
{
	return glm::vec4(
		j["x"].get<float>(),
		j["y"].get<float>(),
		j["z"].get<float>(),
		j["w"].get<float>()
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
glm::lvec2 JSON::Get(const json_t& j)
{
	return glm::lvec2(
		j["x"].get<std::int64_t>(),
		j["y"].get<std::int64_t>()
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

template<>
unsigned char JSON::Get(const json_t& j)
{
	return (unsigned char)j.get<int>();
}

template <>
CollisionBox JSON::Get<CollisionBox>(const json_t& j)
{
	glm::lvec2 center;
	JSON::TryGetMember(j, "center", center);
	glm::lvec2 extents;
	JSON::TryGetMember(j, "extents", extents);
	return CollisionBox(center, extents);
}
