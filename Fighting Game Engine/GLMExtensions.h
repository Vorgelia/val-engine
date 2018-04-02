#pragma once
#include<GLM/glm.hpp>
#include <cstdint>

namespace glm
{
	template<typename T>
	T lerp(T from, T to, double amount, bool clamp = true)
	{
		if(clamp)
		{
			amount = glm::clamp<double>(amount, 0.0, 1.0);
		}
		return from + (to - from)*amount;
	}

	template<typename T>
	T moveTowards(T from, T to, T rate)
	{
		if (to == from)
		{
			return from;
		}

		T toTarget = to - from;
		toTarget = glm::sign(toTarget) * glm::sign(rate) * glm::min(glm::abs(toTarget), glm::abs(rate));

		return from + toTarget;
	}

	typedef glm::tvec2<std::int64_t, glm::highp> lvec2;
	typedef glm::tvec4<std::int64_t, glm::highp> lvec4;
}