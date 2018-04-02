#pragma once
#include "BaseService.h"
#include "GLIncludes.hpp"
#include <GLM/glm.hpp>
#include "Delegate.h"

//Controls whether a single render buffer should be used
//#define VE_USE_SINGLE_BUFFER

class Screen : public BaseService
{
private:
	glm::ivec2 _nativeFramebufferSize;

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

	void Init() override;
	void Update() override;
	void Cleanup() override;

	void HandleResized(GLFWwindow* wnd, int sizeX, int sizeY);

	void SetActiveWindow(GLFWwindow* window);

	Screen(ServiceManager* serviceManager);
	~Screen();
};