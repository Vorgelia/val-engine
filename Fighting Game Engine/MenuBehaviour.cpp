#include "MenuBehaviour.h"
#include "Texture.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "Material.h"
#include "GameInstance.h"
#include "InputManager.h"
#include "InputFrame.h"
#include "InputDevice.h"
#include "CircularBuffer.h"
#include "Camera.h"
#include "ScreenManager.h"
#include "DebugLog.h"

VE_OBJECT_DEFINITION(MenuBehaviour);

void MenuBehaviour::OnInit()
{
	_rendering = _owningInstance->Rendering();
	_screen = _owningInstance->ScreenManager();
	_input = _owningInstance->Input();
	_resource = _owningInstance->ResourceManager();

	_fadingOverlay = std::make_unique<MaterialTexture>(_resource->GetTexture("black"));

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::FrameUpdate, UpdateType::AnyFixedGameUpdate, OnGameUpdate);
	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::UI, UpdateType::LastFixedGameUpdate, OnRenderUI);
}

void MenuBehaviour::OnRenderUI()
{
	double currentTime = owningScene()->GetTime().time();
	double deltaTime = owningScene()->GetTime().deltaTime();

	_rendering->DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(glm::min<double>((int)std::round(1.0 / deltaTime), 60)), nullptr);
	_rendering->DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(deltaTime), nullptr);
	_rendering->DrawScreenText(glm::vec4(0, 50, 100, 100), 24, std::to_string(_owningInstance->updateDispatcher().gameUpdatesPerFrame()), nullptr);

	int ind = 0;
	for(auto& i : _input->inputDevices())
	{
		_rendering->DrawScreenText(glm::vec4(0, 70 + ind * 30, 100, 100), 24, std::to_string(i.first) + ":" + std::to_string(int(i.second->inputBuffer()->back().buttonStates())) + ":" + std::to_string(int(i.second->inputBuffer()->back().axisState())), nullptr);
		++ind;
	}

	if(glm::fract(currentTime) < 0.5f)
		_rendering->DrawScreenText(glm::vec4(0, 1080 - 200, 1920, 60), 140, "VIDEOGAME", nullptr, TextAlignment::Center);

	//if(currentTime < 1)
	//{
	//	_rendering->DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, { *_fadingOverlay }, (Material*)nullptr);
	//}
}

void MenuBehaviour::OnGameUpdate()
{
	ObjectReference<BaseCamera> camera = owningScene()->FindObjectOfType<BaseCamera>();
	if(!camera.IsValid())
	{
		return;
	}

	float deltaTime = float(_owningInstance->configData().gameConfigData.fixedGameUpdateInterval);
	camera->localTransform().SetPosition(camera->localTransform().GetPosition() + ve::vec3(_input->GetInputDevice(-1)->inputBuffer()->back().ToVector() * 30.0f * float(deltaTime), 0));

	OrthoCamera* orthoCamera = dynamic_cast<OrthoCamera*>(camera.get());
	if(orthoCamera == nullptr)
	{
		return;
	}

	if(glfwGetKey(_screen->window(), GLFW_KEY_E) == GLFW_PRESS)
	{
		orthoCamera->SetZoomLevel(orthoCamera->zoomLevel() + deltaTime);
	}
	if(glfwGetKey(_screen->window(), GLFW_KEY_Q) == GLFW_PRESS)
	{
		orthoCamera->SetZoomLevel(orthoCamera->zoomLevel() - deltaTime);
	}
}