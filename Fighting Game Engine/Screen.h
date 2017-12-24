#pragma once
#include "BaseService.h"
#include <vector>
#include "GLIncludes.hpp"
#include <GLM\glm.hpp>
#include "Delegate.h"

class Screen : public BaseService
{
public:
	typedef Delegate<> ScreenUpdateEventHandler;
	ScreenUpdateEventHandler ScreenUpdated;

	GLFWmonitor* primaryMonitor;
	const GLFWvidmode* mode;
	GLFWwindow* window;
	glm::vec2 mousePos;
	glm::ivec2 size;
	glm::vec2 invSize;
	glm::ivec4 viewportSize;
	double windowAspect;
	double windowAspectInv;

	const double targetAspect;
	const double targetAspectInv;

	void Update();
	void HandleResized(GLFWwindow* wnd, int width, int height);

	Screen(ServiceManager* serviceManager);
	~Screen() = default;
};