#include "InputManager.h"
#include "InputDevice.h"
#include "InputFrame.h"
#include "CommonUtilIncludes.hpp"
#include "ResourceLoader.h"
#include "Screen.h"
namespace InputManager{
	std::map<int,InputDevice*> inputDevices;
	std::thread inputCollectionThread;
	int p1Device;
	int p2Device;
	bool stopInputs=false;
}
//Update list of valid input devices and have them poll for inputs
void InputManager::Update(){
	glfwPollEvents();
	//stopInputs = true;
	//inputCollectionThread.join();
	for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i){
		if (glfwJoystickPresent(i)==GLFW_TRUE && (inputDevices.count(i) == 0)){
			inputDevices.insert(std::pair<int, InputDevice*>(i, new InputDevice(i)));
			p2Device = i;//For testing. Remove later.
		}
		else if (glfwJoystickPresent(i) == GLFW_FALSE && (inputDevices.count(i) > 0)){
			delete inputDevices.at(i);
			inputDevices.erase(i);
			if (p1Device == i)
				p1Device = -2;
			if (p2Device == i)
				p2Device = -2;
		}
	}
	
	for (auto i = inputDevices.begin(); i!=inputDevices.end(); ++i){
		if (i->second != nullptr){
			i->second->UpdateJoyInputs();
			i->second->PollInput();
			i->second->PushInputsToBuffer();
		}
	}
	//inputCollectionThread = std::thread(BufferInputs);
}
void InputManager::BufferInputs(){
	/*stopInputs = false;
	do{
		for (auto i = inputDevices.begin(); i != inputDevices.end(); ++i){
			if (i->second != nullptr){
				i->second->UpdateJoyInputs();
				i->second->PollInput();
			}
		}
		
	} while (stopInputs == false);*/
}

void InputManager::Init(){
	inputDevices[-1] = new InputDevice(-1);
	p1Device = -1;
	p2Device = 0;
	//inputCollectionThread = std::thread(BufferInputs);
}

void InputManager::Cleanup(){
	//stopInputs = true;
	//inputCollectionThread.join();
	for (auto i = inputDevices.begin(); i != inputDevices.end();++i){
		delete(i->second);
	}
	inputDevices.clear();
}

void InputManager::KeyCallback(GLFWwindow* window,int key, int scancode,int action,int mods){
	//std::cout << "---Key:" << glfwGetTime()<<"/"<<Time::lastUpdateTime << " - " << (glfwGetKey(Screen::window, key) == GLFW_PRESS) << std::endl;
}