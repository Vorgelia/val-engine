#include "IntroBehaviour.h"
#include "ServiceManager.h"
#include "Time.h"
#include "ResourceManager.h"
#include "RenderingGL.h"
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
	_debug->VE_LOG("Pause time: " + std::to_string(glfwGetTime()));
}

void IntroBehaviour::OnRenderUI()
{
	_resource->GetMaterial("Materials/Intro/Intro_Screen.vmat")->uniformVectors["ve_color"].a = glm::clamp<float>(glm::min<float>((float)_time->timeSinceLoad, _introDuration - (float)_time->timeSinceLoad), 0.0f, 1.0f);
	_rendering->DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, _resource->GetMaterial("Materials/Intro/Intro_Screen.vmat"));

	_rendering->DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(glm::min<double>((int)std::round(1.0 / _time->smoothDeltaTime), 60)), nullptr);
	_rendering->DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(glm::max<double>(((int)(_time->smoothUpdateRate * 100))*0.01, 1.0)), nullptr);

	int ind = 0;
	for(auto& i : _input->inputDevices())
	{
		_rendering->DrawScreenText(glm::vec4(0, 60 + ind * 30, 100, 100), 24, std::to_string(i.first) + ":" + std::to_string(i.second->inputBuffer()->back().buttonStates()) + ":" + std::to_string(i.second->inputBuffer()->back().axisState()), nullptr);
		++ind;
	}
}

void IntroBehaviour::GameUpdate()
{
	bool playerInput = false;
	for(auto& i : _input->inputDevices())
	{
		if(i.second->inputBuffer()->back().buttonStates() > 0)
		{
			playerInput = true;
			break;
		}
	}

	if((float)_time->timeSinceLoad > _introDuration || playerInput)
		_gameSceneManager->LoadScene("Menu");
}

IntroBehaviour::IntroBehaviour(Object * owner, ServiceManager* serviceManager) : Behaviour(owner, serviceManager)
{
	enabled = true;
}

IntroBehaviour::IntroBehaviour(Object * owner, ServiceManager* serviceManager, const json & j) : Behaviour(owner, serviceManager, j)
{
	_debug = serviceManager->Debug();
	_input = serviceManager->Input();
	_rendering = serviceManager->Rendering();
	_gameSceneManager = serviceManager->GameSceneManager();
	_time = serviceManager->Time();
	_resource = serviceManager->ResourceManager();

	_introDuration = JSON::Get<float>(j["introDuration"]);
}
