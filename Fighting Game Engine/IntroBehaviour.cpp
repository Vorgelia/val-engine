#include "IntroBehaviour.h"
#include "Time.h"
#include "Resource.h"
#include "Rendering.h"
#include "Material.h"
#include "InputManager.h"
#include "InputFrame.h"
#include "InputDevice.h"
#include "InputMotion.h"
#include "GameSceneManager.h"
#include "Screen.h"
#include "DebugLog.h"
#include <GLM\glm.hpp>
#include "BehaviourFactory.h"

VE_BEHAVIOUR_REGISTER_TYPE(IntroBehaviour);

void IntroBehaviour::OnSceneInit()
{
	VE_DEBUG_LOG("Pause time: " + std::to_string(glfwGetTime()));
}

void IntroBehaviour::OnRenderUI()
{
	Resource::GetMaterial("Materials/Intro/Intro_Screen.vmat")->uniformVectors["ve_color"].a = glm::clamp<float>(glm::min<float>((float)Time::timeSinceLoad, _introDuration - (float)Time::timeSinceLoad), 0.0f, 1.0f);
	Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, Resource::GetMaterial("Materials/Intro/Intro_Screen.vmat"));

	Rendering::DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(glm::min<double>((int)std::round(1.0 / Time::smoothDeltaTime), 60)), nullptr);
	Rendering::DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(glm::max<double>(((int)(Time::updateRate * 100))*0.01, 1.0)), nullptr);

	int ind = 0;
	for(auto& i : InputManager::inputDevices())
	{
		Rendering::DrawScreenText(glm::vec4(0, 60 + ind * 30, 100, 100), 24, std::to_string(i.first) + ":" + std::to_string(i.second->inputBuffer()->back().buttonStates()) + ":" + std::to_string(i.second->inputBuffer()->back().axisState()), nullptr);
		++ind;
	}
}

void IntroBehaviour::GameUpdate()
{
	bool playerInput = false;
	for(auto& i : InputManager::inputDevices())
	{
		if(i.second->inputBuffer()->back().buttonStates() > 0)
		{
			playerInput = true;
			break;
		}
	}

	if((float)Time::timeSinceLoad > _introDuration || playerInput)
		GameSceneManager::LoadScene("Menu");
}

IntroBehaviour::IntroBehaviour(Object * owner) : Behaviour(owner)
{
	enabled = true;
}

IntroBehaviour::IntroBehaviour(Object * owner, const json & j) : Behaviour(owner, j)
{
	_introDuration = JSON::Get<float>(j["introDuration"]);
}
