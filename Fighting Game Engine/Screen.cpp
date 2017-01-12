#include "Screen.h"

namespace Screen{
	GLFWmonitor* primaryMonitor;
	const GLFWvidmode* mode;
	GLFWwindow* window;
	glm::vec2 mousePos;
	glm::ivec2 size;
	glm::vec2 invSize;
	glm::ivec4 viewportSize;
	double windowAspect;
	double windowAspectInv;
	std::vector<void(*)()> screenUpdateCallbacks;

	const double targetAspect=16.0/9.0;
	const double targetAspectInv=9.0/16.0;
}
//Management of window related stuff. Probably not going to change at all from here.
void Screen::Init(){
	Screen::size = glm::ivec2(1280, 320);
	Screen::invSize = glm::vec2(1.0 / 1280.0, 1.0 / 720.0);
	windowAspect = Screen::size.x / Screen::size.y;
	windowAspectInv = 1.0 / windowAspect;
	Screen::primaryMonitor = glfwGetPrimaryMonitor();
	Screen::mode = glfwGetVideoMode(Screen::primaryMonitor);

	if (Screen::targetAspect > Screen::windowAspect){
		viewportSize = glm::ivec4(0.0, (Screen::size.y - Screen::size.x*Screen::targetAspectInv)*0.5, Screen::size.x, Screen::size.x*Screen::targetAspectInv);
	}
	else if (Screen::targetAspect < Screen::windowAspect) {
		viewportSize = glm::ivec4((Screen::size.x - Screen::size.y*Screen::targetAspect)*0.5, 0, Screen::size.y*Screen::targetAspect, Screen::size.y);
	}
}
void Screen::Update(){
	double mousex, mousey;
	glfwGetCursorPos(Screen::window,&mousex,&mousey);
	Screen::mousePos = glm::vec2(mousex, mousey);
}

void Screen::OnResize(GLFWwindow* wnd, int sizeX, int sizeY){
	size = glm::ivec2(sizeX, sizeY);
	invSize = glm::vec2(1.0 / (float)sizeX, 1.0 / (float)sizeY);
	windowAspect = Screen::size.x / Screen::size.y;
	windowAspectInv = 1.0 / windowAspect;

	if (Screen::targetAspect > Screen::windowAspect){
		viewportSize=glm::ivec4(0.0, (Screen::size.y - Screen::size.x*Screen::targetAspectInv)*0.5, Screen::size.x, Screen::size.x*Screen::targetAspectInv);
	}
	else if (Screen::targetAspect < Screen::windowAspect) {
		viewportSize = glm::ivec4((Screen::size.x - Screen::size.y*Screen::targetAspect)*0.5, 0, Screen::size.y*Screen::targetAspect,Screen::size.y);
	}

	for (auto i = screenUpdateCallbacks.begin(); i != screenUpdateCallbacks.end(); ++i)
		(*i)();
}
