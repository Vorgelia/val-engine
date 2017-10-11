#pragma once
#include <vector>
#include "GLIncludes.hpp"
#include <GLM\glm.hpp>

namespace Screen
{
	extern GLFWmonitor* primaryMonitor;
	extern const GLFWvidmode* mode;
	extern GLFWwindow* window;
	extern glm::vec2 mousePos;
	extern glm::ivec2 size;
	extern glm::vec2 invSize;
	extern glm::ivec4 viewportSize;
	extern double windowAspect;
	extern double windowAspectInv;
	extern std::vector<void(*)()> screenUpdateCallbacks;

	extern const double targetAspect;
	extern const double targetAspectInv;

	void Init();
	void Update();
	void OnResize(GLFWwindow* wnd, int width, int height);
}
