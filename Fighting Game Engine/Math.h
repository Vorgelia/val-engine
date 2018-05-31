#pragma once
#include "MathIncludes.hpp"

namespace ValEngine
{
	typedef FixedPoint64 dec_t;
	typedef std::int32_t int_t;
	typedef std::uint32_t uint_t;

	typedef glm::tvec2<dec_t> vec2;
	typedef glm::tvec3<dec_t> vec3;
	typedef glm::tvec4<dec_t> vec4;

	typedef glm::tvec2<int_t> ivec2;
	typedef glm::tvec3<int_t> ivec3;
	typedef glm::tvec4<int_t> ivec4;
}