#pragma once
#include<GLM\glm.hpp>
#include<GLM\gtc\matrix_transform.hpp>
#include<GLM\gtc\type_ptr.hpp>

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
}