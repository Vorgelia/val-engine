#include "IntroBehaviour.h"
#include "GameInstance.h"
#include "ResourceManager.h"
#include "RenderingGL.h"
#include "Material.h"
#include "InputManager.h"
#include "InputFrame.h"
#include "InputDevice.h"
#include "GameSceneManager.h"
#include "ScreenManager.h"
#include "DebugLog.h"
#include <GLM/glm.hpp>

VE_OBJECT_DEFINITION(IntroBehaviour);

void IntroBehaviour::RegisterReflectionFields() const
{
	VE_PRIVATE_REFLECTION_VAR(Field, introDuration);
}

void IntroBehaviour::OnInit()
{
	_debug = _owningInstance->Debug();
	_input = _owningInstance->Input();
	_rendering = _owningInstance->Rendering();
	_gameSceneManager = _owningInstance->GameSceneManager();
	_resource = _owningInstance->ResourceManager();

	_debug->VE_LOG("Pause time: " + std::to_string(glfwGetTime()));

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::FrameUpdate, UpdateType::AnyFixedGameUpdate, OnGameUpdate);
	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::UI, UpdateType::LastFixedGameUpdate, OnRenderUI);
}

void IntroBehaviour::OnRenderUI()
{
	Material* screenMat = _resource->GetMaterial("Materials/Intro/Intro_Screen.vmat");
	double currentTime = owningScene()->GetTime().time();

	screenMat->uniformVectors["ve_color"].a = glm::clamp<float>(glm::min<float>(currentTime, _introDuration - currentTime), 0.0f, 1.0f);
	_rendering->DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, screenMat);

	double deltaTime = owningScene()->GetTime().deltaTime();

	_rendering->DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(glm::min<double>((int)std::round(1.0 / deltaTime), 60)), nullptr);
	_rendering->DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(glm::max<double>(((int)((1.0f / deltaTime) * 100))*0.01f, 1.0)), nullptr);

	int ind = 0;
	for(auto& i : _input->inputDevices())
	{
		_rendering->DrawScreenText(glm::vec4(0, 60 + ind * 30, 100, 100), 24, std::to_string(i.first) + ":" + std::to_string(int(i.second->inputBuffer()->back().buttonStates())) + ":" + std::to_string(int(i.second->inputBuffer()->back().axisState())), nullptr);
		++ind;
	}
}

void IntroBehaviour::OnGameUpdate()
{
	bool playerInput = false;
	for(auto& i : _input->inputDevices())
	{
		if(i.second->inputBuffer()->back().buttonStates() != InputButton::None)
		{
			playerInput = true;
			break;
		}
	}

	float currentTime = owningScene()->GetTime().time();
	if(currentTime > _introDuration || playerInput)
		_gameSceneManager->LoadScene("Menu");
}