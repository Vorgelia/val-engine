#pragma once
#include "CommonUtilIncludes.hpp"

namespace Profiler{
	extern std::unordered_map<std::string, double> elements;
	extern glm::vec2 stateChanges;
	void Begin(std::string str);
	void End(std::string str);
	void Clear();
	std::string String(std::string str);
	void Print();
}