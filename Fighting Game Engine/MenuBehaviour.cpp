#include "MenuBehaviour.h"
#include "Texture.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "Material.h"
#include "GameInstance.h"
#include "Screen.h"
#include "Time.h"
#include "Transform.h"
#include "InputManager.h"
#include "InputFrame.h"
#include "InputDevice.h"
#include "CircularBuffer.h"
#include "Camera.h"
#include "BehaviourFactory.h"

VE_BEHAVIOUR_REGISTER_TYPE(MenuBehaviour);

void MenuBehaviour::OnRenderUI()
{
	_rendering->DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(glm::min<double>((int)std::round(1.0 / _time->smoothDeltaTime), 60)), nullptr);
	_rendering->DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(glm::max<double>(((int)(_time->smoothUpdateRate * 100))*0.01, 1.0)), nullptr);

	if(glm::fract(_time->time) < 0.5f)
		_rendering->DrawScreenText(glm::vec4(0, 1080 - 200, 1920, 60), 140, "VIDEOGAME", nullptr, TextAlignment::Center);

	if(_time->timeSinceLoad < 1)
	{
		_rendering->tintColor.a = (float)(1.0 - (_time->timeSinceLoad));
		_rendering->DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, { *_fadingOverlay }, (Material*)nullptr);
	}
}

void MenuBehaviour::EngineUpdate()
{
	if(glfwGetKey(_screen->window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(_screen->window, GLFW_TRUE);
	}
}

void MenuBehaviour::GameUpdate()
{
	_rendering->cameras.at(0).position += _input->GetInputDevice(-1)->inputBuffer()->back().ToVector() * 30.0f * float(VE_FRAME_TIME);
	if(glfwGetKey(_screen->window, GLFW_KEY_E) == GLFW_PRESS)
	{
		_rendering->cameras.at(0).zoomLevel += 1.0f * float(VE_FRAME_TIME);
	}	
	if(glfwGetKey(_screen->window, GLFW_KEY_Q) == GLFW_PRESS)
	{
		_rendering->cameras.at(0).zoomLevel -= 1.0f * float(VE_FRAME_TIME);
	}
}

MenuBehaviour::MenuBehaviour(Object* owner, GameInstance* serviceManager, const json& j) : Behaviour(owner, serviceManager, j)
{
	_rendering = serviceManager->Rendering();
	_screen = serviceManager->Screen();
	_input = serviceManager->Input();
	_time = serviceManager->Time();
	_resource = serviceManager->ResourceManager();

	_fadingOverlay = std::make_unique<MaterialTexture>(_resource->GetTexture("black"));
}

MenuBehaviour::~MenuBehaviour()
= default;
