#include "GS_Menu.h"
#include "Rendering.h"
#include "Resource.h"
#include "Texture.h"
#include "CommonUtilIncludes.hpp"
#include "Screen.h"
#include "Time.h"
#include "InputManager.h"
#include "InputFrame.h"
#include "InputDevice.h"
#include "InputMotion.h"
void GS_Menu::FrameEnd(){
	Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), Resource::GetMesh("Meshes/Base/screenQuad.vm"), std::vector<Texture*>{ Resource::GetTexture("Textures/tex.png") }, Resource::GetMaterial("Materials/Base/Screen.vmat"));
}

void GS_Menu::GUI(){
	Rendering::DrawScreenText(glm::vec4(0, 10, 100, 100), 24, std::to_string(std::min((int)std::round(1.0 / Time::smoothDeltaTime), 60)), nullptr);
	Rendering::DrawScreenText(glm::vec4(0, 30, 100, 100), 24, std::to_string(std::max(((int)(Time::updateRate * 100))*0.01, 1.0)), nullptr);
	
	if ((int)(Time::time*4)%2==1)
		Rendering::DrawScreenText(glm::vec4(0, 1080-200, 1920, 60), 140, "SO MUCH VIDEOGAME", nullptr,TextAlignment::Center);


	if (Time::timeSinceLoad < 1){
		Rendering::tintColor.a = (float)(1.0-(Time::timeSinceLoad));
		Rendering::DrawScreenMesh(glm::vec4(0, 0, 1920, 1080), (Mesh*)nullptr, {Resource::GetTexture("black")},(Material*)nullptr);
	}
	int ind = 0;
	for (auto i = InputManager::inputDevices.begin(); i != InputManager::inputDevices.end(); ++i){
		Rendering::DrawScreenText(glm::vec4(0, 60 + ind * 30, 100, 100), 24, std::to_string(i->first) + ":" + std::to_string(i->second->LastBufferInput()->buttonStates) + ":" + std::to_string(i->second->LastBufferInput()->axisState), nullptr);
		++ind;
	}
}
InputMotion qcf = {
	InputMotionComponent({}, 2, 0, 20, false),
	InputMotionComponent({}, 6, 0, 20, true),
	InputMotionComponent({}, 4, 0, 20, false),
	InputMotionComponent({ InputButtonEvent((unsigned char)InputButton::Light, InputType::Pressed) }, 0, 0, 1, false)
};
void GS_Menu::Update(){
	if (glfwGetKey(Screen::window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
		glfwSetWindowShouldClose(Screen::window, GLFW_TRUE);
	}
}
void GS_Menu::GameUpdate(){
	InputManager::inputDevices[0]->EvaluateMotion(qcf, false);
}
GS_Menu::GS_Menu(FS::path path) :GameState(path){
}

void GS_Menu::Init(){
	_initialized = true;
	
	
}