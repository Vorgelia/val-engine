#include "MenuBehaviour.h"
#include "Rendering.h"
#include "Resource.h"
#include "Texture.h"
#include "CommonUtilIncludes.hpp"
#include "Screen.h"
#include "Time.h"
#include "Transform.h"
#include "InputManager.h"
#include "InputFrame.h"
#include "InputDevice.h"
#include "InputMotion.h"
#include "CircularBuffer.h"
#include "Camera.h"
#include "BehaviourFactory.h"

VE_BEHAVIOUR_REGISTER_TYPE(MenuBehaviour);

void MenuBehaviour::OnRenderUI()
{
	Rendering::DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(glm::min<double>((int)std::round(1.0 / Time::smoothDeltaTime), 60)), nullptr);
	Rendering::DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(glm::max<double>(((int)(Time::updateRate * 100))*0.01, 1.0)), nullptr);

	if((int)(Time::time * 4) % 2 == 1)
		Rendering::DrawScreenText(glm::vec4(0, 1080 - 200, 1920, 60), 140, "VIDEOGAME", nullptr, TextAlignment::Center);


	int ind = 0;
	for(auto i = InputManager::_inputDevices.begin(); i != InputManager::_inputDevices.end(); ++i)
	{
		if(i->second != nullptr)
			Rendering::DrawScreenText(glm::vec4(0, 60 + ind * 30, 100, 100), 24, std::to_string(i->first) + ":" + std::to_string(i->second->inputBuffer()->back()->buttonStates) + ":" + std::to_string(i->second->inputBuffer()->back()->axisState), nullptr);
		++ind;
	}

	if(Time::timeSinceLoad < _motionTimer)
	{
		Rendering::DrawScreenText(glm::vec4(0, 1080 - 400, 1920, 60), 140, "MOTION", nullptr, TextAlignment::Center);
	}

	if(Time::timeSinceLoad < 1)
	{
		Rendering::tintColor.a = (float)(1.0 - (Time::timeSinceLoad));
		Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, { Resource::GetTexture("black") }, (Material*)nullptr);
	}

}

//Sonic boom motion

//TODO: Fix negative edge
InputMotion motion = {
	InputMotionComponent(std::vector<InputButtonEvent>{}, (unsigned char)InputDirection::Left, 30, 20, false),
	InputMotionComponent(std::vector<InputButtonEvent>{}, (unsigned char)InputDirection::Right, 0, 10, false),
	InputMotionComponent(std::vector<InputButtonEvent>{ InputButtonEvent((unsigned char)InputButton::Light, InputType::Released) }, 0, 0, 10, false)
};

void MenuBehaviour::Update()
{
	if(glfwGetKey(Screen::window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
	}

}

void MenuBehaviour::GameUpdate()
{
	if(InputManager::_inputDevices[-1] != nullptr)
	{
		if(InputManager::_inputDevices[-1]->EvaluateMotion(motion, false))
		{
			_motionTimer = (float)Time::timeSinceLoad + 1;
		}
	}
	Rendering::cameras.at(0).position += InputManager::_inputDevices[-1]->inputBuffer()->back()->ToVector() * 500.0f * (float)VE_FRAME_TIME;
}

MenuBehaviour::MenuBehaviour(Object* owner, const json& j) : Behaviour(owner, j)
{
}

MenuBehaviour::~MenuBehaviour()
{
}
