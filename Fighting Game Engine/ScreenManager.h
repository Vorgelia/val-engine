#pragma once
#include "BaseService.h"
#include "GLIncludes.hpp"
#include <GLM/glm.hpp>
#include "Delegate.h"

class ScreenManager : public BaseService
{
	VE_OBJECT_DECLARATION(ScreenManager);

protected:
	glm::ivec2 _nativeFramebufferSize;

	GLFWmonitor* _primaryMonitor;
	const GLFWvidmode* _videoMode;
	GLFWwindow* _window;

	glm::ivec2 _screenSize;
	glm::vec2 _screenInvSize;

	glm::ivec4 _viewportSize;

	double _windowAspect;
	double _windowInvAspect;
	double _windowTargetAspect;
	double _windowInvTargetAspect;

	glm::dvec2 _mousePosition;

public:
	typedef Delegate<> ScreenUpdateEventHandler;
	ScreenUpdateEventHandler ScreenUpdated;

	GLFWwindow* window() const { return _window; }
	
	void OnInit() override;
	void OnDestroyed() override;

	void Update();

	void HandleResized(GLFWwindow* wnd, int sizeX, int sizeY);
	void SetActiveWindow(GLFWwindow* window);

	ScreenManager();
	~ScreenManager() = default;
};