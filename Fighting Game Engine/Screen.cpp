#include "Screen.h"
#include "Delegate.h"

void Screen::Init()
{
}

void Screen::Update()
{
	double mousex, mousey;
	glfwGetCursorPos(window, &mousex, &mousey);
	mousePos = glm::vec2(mousex, mousey);

	glm::ivec2 framebufferSize;
	glfwGetFramebufferSize(window, &framebufferSize.x, &framebufferSize.y);

	if(_nativeFramebufferSize != framebufferSize)
	{
		_nativeFramebufferSize = framebufferSize;
		HandleResized(window, framebufferSize.x, framebufferSize.y);
	}
}

void Screen::Cleanup()
{
	glfwTerminate();
}

void Screen::HandleResized(GLFWwindow* wnd, int sizeX, int sizeY)
{
	size = glm::ivec2(sizeX, sizeY);
	invSize = glm::vec2(1.0 / (float)sizeX, 1.0 / (float)sizeY);

	windowAspect = float(size.x) / float(size.y);
	windowAspectInv = 1.0 / windowAspect;

	if(targetAspect > windowAspect)
	{
		viewportSize = glm::ivec4(0.0, (size.y - size.x*targetAspectInv) / 2, size.x, size.x * targetAspectInv);
	}
	else if(targetAspect < windowAspect)
	{
		viewportSize = glm::ivec4((size.x - size.y*targetAspect) / 2, 0, size.y * targetAspect, size.y);
	}

	ScreenUpdated();
}

void Screen::SetActiveWindow(GLFWwindow* window)
{
	this->window = window;
	glfwGetWindowSize(window, &_nativeFramebufferSize.x, &_nativeFramebufferSize.y);

	HandleResized(window, _nativeFramebufferSize.x, _nativeFramebufferSize.y);

	primaryMonitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(primaryMonitor);

	glfwMakeContextCurrent(window);
	glfwSetWindowPos(window, (int)(mode->width*0.5 - size.x*0.5), (int)(mode->height*0.5 - size.y*0.5));
	glfwSwapInterval(0);
}

Screen::Screen(GameInstance* serviceManager) :
	BaseService(serviceManager, -50),
	targetAspect(16.0 / 9.0),
	targetAspectInv(9.0 / 16.0)
{
	glfwInit();
	_allowServiceUpdate = true;

	//Window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);
#ifdef VE_USE_SINGLE_BUFFER
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
#endif

	//Initialize and store window
	SetActiveWindow(glfwCreateWindow(1280, 720, "Videogame", nullptr, nullptr));

	glewExperimental = true;
	glewInit();
}

Screen::~Screen()
= default;
