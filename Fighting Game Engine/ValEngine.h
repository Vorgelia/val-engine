#pragma once
#include <cstdint>
#include "MathIncludes.hpp"

//TODO: Add/move more stuff here.

namespace ValEngine
{
	typedef FixedPoint64 dec_t;
	typedef std::int32_t int_t;

	typedef glm::tvec2<dec_t> vec2;
	typedef glm::tvec3<dec_t> vec3;
	typedef glm::tvec4<dec_t> vec4;

	typedef glm::tvec2<int_t> ivec2;
	typedef glm::tvec3<int_t> ivec3;
	typedef glm::tvec4<int_t> ivec4;
}

namespace ve = ValEngine;