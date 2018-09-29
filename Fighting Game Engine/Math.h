#pragma once
#include "MathIncludes.hpp"

namespace ValEngine
{
	typedef FixedPoint64 dec_t;
	typedef std::int32_t int_t;
	typedef std::uint32_t uint_t;

	typedef std::uint8_t uint8_t;

	typedef glm::tvec2<dec_t> vec2;
	typedef glm::tvec3<dec_t> vec3;
	typedef glm::tvec4<dec_t> vec4;

	typedef glm::vec2 fvec2;
	typedef glm::vec3 fvec3;
	typedef glm::vec4 fvec4;

	typedef glm::tvec2<int_t> ivec2;
	typedef glm::tvec3<int_t> ivec3;
	typedef glm::tvec4<int_t> ivec4;

	typedef glm::tquat<dec_t> quat;

	typedef glm::tmat3x3<dec_t> mat3;
	typedef glm::tmat4x4<dec_t> mat4;
}