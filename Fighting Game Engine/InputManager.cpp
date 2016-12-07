#include "InputManager.h"
#include "InputDevice.h"
#include "InputFrame.h"
#include "CommonUtilIncludes.hpp"
#include "ResourceLoader.h"
namespace InputManager{
	std::map<int,InputDevice*> inputDevices;
	std::thread inputCollectionThread;
	int p1Device;
	int p2Device;
	bool stopInputs=false;
}

void InputManager::Update(){
	glfwPollEvents();

	for (int i = GLFW_JOYSTICK_1; i < GLFW_JOYSTICK_LAST; ++i){
		if (glfwJoystickPresent(i)==GLFW_TRUE && (inputDevices.count(i) == 0)){
			inputDevices.insert(std::pair<int, InputDevice*>(i, new InputDevice(i)));
			p2Device = i;
		}
		else if (glfwJoystickPresent(i) == GLFW_FALSE && (inputDevices.count(i) > 0)){
			delete inputDevices[i];
			inputDevices.erase(i);
			if (p1Device == i)
				p1Device = -2;
			if (p2Device == i)
				p2Device = -2;
		}
	}
	for (auto i = inputDevices.begin(); i!=inputDevices.end(); ++i){
		i->second->UpdateJoyInputs();
		i->second->PollInput();
		i->second->PushInputsToBuffer();
	}
}
void InputManager::BufferInputs(){
	//TODO: Uncomment and call on a separate thread when i write my own input handler
	//do{
		//if (inputDevices.count(p1Device)>0)
			//inputDevices[p1Device]->PollInput();
		//if (inputDevices.count(p2Device)>0)
			//inputDevices[p2Device]->PollInput();
		
	//} while (stopInputs == false);
}

void InputManager::Init(){
	inputDevices[-1] = new InputDevice(-1);
	p1Device = -1;
	p2Device = 0;
}

void InputManager::Cleanup(){
	for (auto i = inputDevices.begin(); i != inputDevices.end();++i){
		delete(i->second);
	}

}

void InputManager::KeyCallback(GLFWwindow* window,int key, int scancode,int action,int mods){
	//std::cout <<"---Key:" <<glfwGetTime() << std::endl;
}