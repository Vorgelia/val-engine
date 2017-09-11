#include "GS_Intro.h"
#include "Time.h"
#include "Resource.h"
#include "Rendering.h"
#include "Material.h"
#include "InputManager.h"
#include "InputFrame.h"
#include "InputDevice.h"
#include "InputMotion.h"
#include "GameStateManager.h"
#include "Screen.h"
#include "DebugLog.h"

void GS_Intro::Init()
{
	DebugLog::Push("Start time: " + std::to_string(glfwGetTime()));
	levelTimer = 0;
	_initialized = true;
}
void GS_Intro::GUI()
{
	Resource::GetMaterial("Materials/Intro/Intro_Screen.vmat")->uniformVectors["ve_color"].a = glm::clamp(glm::min(levelTimer, 4 - levelTimer), 0.0f, 1.0f);
	Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, Resource::GetMaterial("Materials/Intro/Intro_Screen.vmat"));

	Rendering::DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(std::min((int)std::round(1.0 / Time::smoothDeltaTime), 60)), nullptr);
	Rendering::DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(std::max(((int)(Time::updateRate * 100))*0.01, 1.0)), nullptr);

	int ind = 0;
	for(auto i = InputManager::inputDevices.begin(); i != InputManager::inputDevices.end(); ++i)
	{
		Rendering::DrawScreenText(glm::vec4(0, 60 + ind * 30, 100, 100), 24, std::to_string(i->first) + ":" + std::to_string(i->second->inputBuffer->back()->buttonStates) + ":" + std::to_string(i->second->inputBuffer->back()->axisState), nullptr);
		++ind;
	}
}

void GS_Intro::GameUpdate()
{
	levelTimer += (float)VE_FRAME_TIME;
	if(levelTimer > 4)
		GameStateManager::LoadState(1);
	else
	{
		for(auto i = InputManager::inputDevices.begin(); i != InputManager::inputDevices.end(); ++i)
		if(i->second->inputBuffer->back()->buttonStates > 0)
			GameStateManager::LoadState(1);
	}
}

GS_Intro::GS_Intro(FS::path path) :GameState(path)
{

}