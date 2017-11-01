#include "GS_Intro.h"
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

void GS_Intro::Init()
{
	DebugLog::Push("Start time: " + std::to_string(glfwGetTime()));
	_levelTimer = 0;
	_initialized = true;
}
void GS_Intro::RenderUI()
{
	GameScene::RenderUI();
	Resource::GetMaterial("Materials/Intro/Intro_Screen.vmat")->uniformVectors["ve_color"].a = glm::clamp<float>(glm::min<float>(_levelTimer, 4 - _levelTimer), 0.0f, 1.0f);
	Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, Resource::GetMaterial("Materials/Intro/Intro_Screen.vmat"));

	Rendering::DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(glm::min<double>((int)std::round(1.0 / Time::smoothDeltaTime), 60)), nullptr);
	Rendering::DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(glm::max<double>(((int)(Time::updateRate * 100))*0.01, 1.0)), nullptr);

	int ind = 0;
	for(auto& i : InputManager::_inputDevices)
	{
		Rendering::DrawScreenText(glm::vec4(0, 60 + ind * 30, 100, 100), 24, std::to_string(i.first) + ":" + std::to_string(i.second->inputBuffer()->back()->buttonStates) + ":" + std::to_string(i.second->inputBuffer()->back()->axisState), nullptr);
		++ind;
	}
}

void GS_Intro::GameUpdate()
{
	GameScene::GameUpdate();
	_levelTimer += (float)VE_FRAME_TIME;

	bool playerInput = false;
	for(auto& i : InputManager::_inputDevices)
	{
		if(i.second->inputBuffer()->back()->buttonStates > 0)
		{
			playerInput = true;
			break;
		}
	}


	if(_levelTimer > 4 || playerInput)
		GameSceneManager::LoadScene("Menu");
}

GS_Intro::GS_Intro(const FS::path& path) :GameScene(path)
{

}