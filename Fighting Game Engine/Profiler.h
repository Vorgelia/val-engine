#pragma once
#include "CommonUtilIncludes.hpp"

namespace Profiler
{
	extern std::unordered_map<std::string, double> _elements;
	extern glm::vec2 _stateChanges;

	void Begin(const std::string& str);
	void End(const std::string& str);
	void Clear();

	std::string String(const std::string& str);

	void Print();
}