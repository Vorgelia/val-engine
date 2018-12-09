#include "ScreenManager.h"
#include "Delegate.h"
#include "GameInstance.h"

VE_OBJECT_DEFINITION(ScreenManager);

void ScreenManager::OnInit()
{
	const RenderingConfigData& renderingConfig = _owningInstance->configData().renderingConfigData;

	glfwInit();

	//Window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);
	if(renderingConfig.useSingleBuffer)
	{
		glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
	}

	_windowTargetAspect = double(renderingConfig.targetAspectRatio.x) / double(renderingConfig.targetAspectRatio.y);
	_windowInvTargetAspect = 1.0 / _windowTargetAspect;

	//Initialize and store window
	SetActiveWindow(glfwCreateWindow(renderingConfig.defaultResolution.x, renderingConfig.defaultResolution.y, _owningInstance->configData().gameConfigData.gameName.c_str(), nullptr, nullptr));

	glewExperimental = true;
	glewInit();

	_owningInstance->updateDispatcher().BindFunction(this, UpdateFunctionTiming(int(UpdateGroup::TimingUpdate) + 1, UpdateType::EngineUpdate), [this]() { Update(); });
}

void ScreenManager::OnDestroyed()
{
	glfwTerminate();
}

void ScreenManager::Update()
{
	double mousex, mousey;
	glfwGetCursorPos(_window, &_mousePosition.x, &_mousePosition.y);

	glm::ivec2 framebufferSize;
	glfwGetFramebufferSize(_window, &framebufferSize.x, &framebufferSize.y);

	if(_nativeFramebufferSize != framebufferSize)
	{
		_nativeFramebufferSize = framebufferSize;
		HandleResized(_window, framebufferSize.x, framebufferSize.y);
	}
}

void ScreenManager::HandleResized(GLFWwindow* wnd, int sizeX, int sizeY)
{
	_screenSize = glm::ivec2(sizeX, sizeY);
	_screenInvSize = 1.0f / glm::vec2(_screenSize);

	_windowAspect = float(_screenSize.x) / float(_screenSize.y);
	_windowInvAspect = 1.0 / _windowAspect;

	if(_windowTargetAspect > _windowAspect)
	{
		_viewportSize = glm::ivec4(0.0, (_screenSize.y - _screenSize.x*_windowInvTargetAspect) / 2, _screenSize.x, _screenSize.x * _windowInvTargetAspect);
	}
	else
	{
		_viewportSize = glm::ivec4((_screenSize.x - _screenSize.y * _viewportSize) / 2, 0, _screenSize.y * _viewportSize, _screenSize.y);
	}

	ScreenUpdated.Invoke();
}

void ScreenManager::SetActiveWindow(GLFWwindow* window)
{
	_window = window;
	glfwGetWindowSize(window, &_nativeFramebufferSize.x, &_nativeFramebufferSize.y);

	HandleResized(window, _nativeFramebufferSize.x, _nativeFramebufferSize.y);

	_primaryMonitor = glfwGetPrimaryMonitor();
	_videoMode = glfwGetVideoMode(_primaryMonitor);

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, (int)(_videoMode->width*0.5 - _screenSize.x*0.5), (int)(_videoMode->height*0.5 - _screenSize.y*0.5));
	glfwSwapInterval(0);
}