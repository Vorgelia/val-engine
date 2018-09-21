#include "JSON.h"

template <>
FixedPoint64 JSON::Get<FixedPoint64>(const json_t& j)
{
	if(!j.is_number())
	{
		return {};
	}

	FixedPoint64 fp64 = FixedPoint64::FromString(j.dump());

	const FixedPoint64 fp64d{ j.get<double>() };
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
ve::ivec2 JSON::Get<ve::ivec2>(const json_t& j)
{
	return ve::ivec2(
		j["x"].get<ve::ivec2::value_type>(),
		j["y"].get<ve::ivec2::value_type>()
	);
}

template <>
ve::ivec3 JSON::Get<ve::ivec3>(const json_t& j)
{
	return ve::ivec3(
		j["x"].get<ve::ivec4::value_type>(),
		j["y"].get<ve::ivec4::value_type>(),
		j["z"].get<ve::ivec4::value_type>()
	);
}

template <>
ve::ivec4 JSON::Get<ve::ivec4>(const json_t& j)
{
	return ve::ivec4(
		j["x"].get<ve::ivec4::value_type>(),
		j["y"].get<ve::ivec4::value_type>(),
		j["z"].get<ve::ivec4::value_type>(),
		j["w"].get<ve::ivec4::value_type>()
	);
}

bool JSON::HasMember(const json_t& j, const std::string& name)
{
	return j.find(name) != j.end();
}
