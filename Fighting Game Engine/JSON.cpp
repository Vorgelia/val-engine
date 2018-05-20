#include "JSON.h"
#include "GameCharacterData.h"
#include "IReflectable.h"

template <>
FixedPoint64 JSON::Get<FixedPoint64>(const json_t& j)
{
	if(!j.is_number())
	{
		return {};
	}

	FixedPoint64 fp64 = FixedPoint64::FromString(j.dump());
	FixedPoint64 fp64d = FixedPoint64(j.get<double>());
	assert(FixedPoint64::Abs(fp64 - fp64d) < FixedPoint64(0.001));

	return fp64;
}

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
unsigned char JSON::Get(const json_t& j)
{
	return unsigned char(j.get<int>());
}

template <>
CollisionBox JSON::Get<CollisionBox>(const json_t& j)
{
	ve::vec2 center;
	JSON::TryGetMember(j, "center", center);
	ve::vec2 extents;
	JSON::TryGetMember(j, "extents", extents);
	ve::vec2 pivotOffset;
	JSON::TryGetMember(j, "pivotOffset", pivotOffset);

	return CollisionBox(center, extents);
}

template <>
ve::vec2 JSON::Get<ve::vec2>(const json_t& j)
{
	return ve::vec2(
		JSON::Get<ve::dec_t>(j["x"]),
		JSON::Get<ve::dec_t>(j["y"])
	);
}

template <>
ve::vec3 JSON::Get<ve::vec3>(const json_t& j)
{
	return ve::vec3(
		JSON::Get<ve::dec_t>(j["x"]),
		JSON::Get<ve::dec_t>(j["y"]),
		JSON::Get<ve::dec_t>(j["z"])
	);
}

template <>
ve::vec4 JSON::Get<ve::vec4>(const json_t& j)
{
	return ve::vec4(
		JSON::Get<ve::dec_t>(j["x"]),
		JSON::Get<ve::dec_t>(j["y"]),
		JSON::Get<ve::dec_t>(j["z"]),
		JSON::Get<ve::dec_t>(j["w"])
	);
}

template <>
glm::vec2 JSON::Get<glm::vec2>(const json_t& j)
{
	return glm::vec2(
		j["x"].get<glm::vec2::value_type>(),
		j["y"].get<glm::vec2::value_type>()
	);
}

template <>
glm::ivec2 JSON::Get<glm::ivec2>(const json_t& j)
{
	return glm::ivec2(
		j["x"].get<glm::ivec2::value_type>(),
		j["y"].get<glm::ivec2::value_type>()
	);
}

template <>
glm::vec4 JSON::Get<glm::vec4>(const json_t& j)
{
	return glm::vec4(
		j["x"].get<glm::vec4::value_type>(),
		j["y"].get<glm::vec4::value_type>(),
		j["z"].get<glm::vec4::value_type>(),
		j["w"].get<glm::vec4::value_type>()
	);
}

template <>
glm::ivec4 JSON::Get<glm::ivec4>(const json_t& j)
{
	return glm::ivec4(
		j["x"].get<glm::ivec4::value_type>(),
		j["y"].get<glm::ivec4::value_type>(),
		j["z"].get<glm::ivec4::value_type>(),
		j["w"].get<glm::ivec4::value_type>()
	);
}
