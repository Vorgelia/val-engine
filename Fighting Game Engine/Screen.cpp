#include "Screen.h"
#include "Delegate.h"

void Screen::Update()
{
	double mousex, mousey;
	glfwGetCursorPos(window, &mousex, &mousey);
	mousePos = glm::vec2(mousex, mousey);
}

void Screen::HandleResized(GLFWwindow* wnd, int sizeX, int sizeY)
{
	size = glm::ivec2(sizeX, sizeY);
	invSize = glm::vec2(1.0 / (float)sizeX, 1.0 / (float)sizeY);

	windowAspect = size.x / size.y;
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

Screen::Screen(ServiceManager* serviceManager) :
	BaseService(serviceManager, -50),
	targetAspect(16.0 / 9.0),
	targetAspectInv(9.0 / 16.0)
{
	_allowServiceUpdate = true;

	size = glm::ivec2(1280, 720);
	invSize = glm::vec2(1.0 / 1280.0, 1.0 / 720.0);

	windowAspect = size.x / size.y;
	windowAspectInv = 1.0 / windowAspect;

	primaryMonitor = glfwGetPrimaryMonitor();
	mode = glfwGetVideoMode(primaryMonitor);

	if(targetAspect > windowAspect)
	{
		viewportSize = glm::ivec4(0.0, (size.y - size.x * targetAspectInv) / 2, size.x, size.x * targetAspectInv);
	}
	else if(targetAspect < windowAspect)
	{
		viewportSize = glm::ivec4((size.x - size.y * targetAspect) / 2, 0, size.y * targetAspect, size.y);
	}
}
