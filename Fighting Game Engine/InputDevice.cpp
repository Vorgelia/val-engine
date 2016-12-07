#include "InputDevice.h"
#include "ResourceLoader.h"
#include "Time.h"
#include "Screen.h"
#include "InputMotion.h"
InputDevice::InputDevice(int deviceID){
	this->_deviceID = deviceID;
	if (deviceID == -2)
		this->_deviceName = std::string("Inactive");
	else{
		if (deviceID == -1)
			this->_deviceName = std::string("Keyboard");
		else
			this->_deviceName = std::string(glfwGetJoystickName(deviceID));
		ResourceLoader::LoadControlSettings("Settings/Input/" + this->_deviceName + ".vi", &(this->directionMap), &(this->buttonMap));
	}
	_inputBuffer.resize(1200);
	_bufferEnd = 1200;
}
bool InputDevice::EvaluateInput(InputEvent ie){
	if (this->_deviceID == -2){
		return false;
	}
	else if (this->_deviceID == -1){
		return glfwGetKey(Screen::window, ie.inputID) == GLFW_PRESS;
	}
	else{
		if (!ie.isAxis){
			return cachedJoyButtons[ie.inputID] > 0;
		}
		else{
			return (glm::abs(cachedJoyAxes[ie.inputID]) > ie.deadzone)&&(glm::sign(cachedJoyAxes[ie.inputID]) == glm::sign(ie.inputValue));
		}
	}
}
//Input leniency
#define INPUT_BUFFER_INIT 1
#define INPUT_BUFFER_MID 8
std::string debugString;
bool InputDevice::EvaluateMotion(InputMotion motion,bool inverse){
	if (motion.size() == 0)
		return false;
	int buf = motion.back().leniency > -1 ? motion.back().leniency : INPUT_BUFFER_INIT;
	int bufferIndex = this->_bufferEnd - 1;
	debugString = "";
	for (int i = motion.size() - 1; i >= 0; ){
		debugString += "Checking frame: " + std::to_string(bufferIndex) + "\n";
		debugString += "-Requirements: " + std::to_string((int)motion[i].direction) + ":";
		for (int bti = 0; bti < motion[i].buttons.size(); ++bti){
			debugString += std::to_string((int)motion[i].buttons[0].first)+",";
		}
		debugString += '\n';
		int distance = InputMotionDistance(bufferIndex,motion[i],buf,i==0);
		if (distance>=0&&distance<=buf){
			debugString += "-Succeeded. Distance="+std::to_string(distance)+"\n";
			bufferIndex -= distance + motion[i].minDuration-1;
			--i;
			if (i <= -1){
				std::cout << "MotionDebug: Frame " << Time::frameCount << std::endl << debugString << "--SUCCESS--\n--------" << std::endl;
				return true;
			}
			else
				buf = motion[i].leniency > -1 ? motion[i].leniency : INPUT_BUFFER_MID;
		}
		else /*if(buf<=-1)*/{
			if (i != motion.size() - 1)
				std::cout << "MotionDebug: Frame " << Time::frameCount << std::endl << debugString << "--FAILED--\n--------" << std::endl;
			return false;
		}
		/*else{
			//debugString += "-Failed\n";
			//bufferIndex -= 1;
			return false;
		}*/
	}
	std::cout << "MotionDebug: " << Time::frameCount << std::endl << debugString << "--FAILED OUTSIDE--\n--------" << std::endl;
	return false;
}

int InputDevice::InputMotionDistance(int currentIndex, InputMotionComponent motionComp,int maxBuffer,bool firstInput){
	int cind = currentIndex;
	int duration = 0;
	do{
		if (InputMotionFrameCheck(&motionComp, cind)){
			debugString += "--Valid Frame " + std::to_string(cind) + " dur:" + std::to_string(duration + 1) + "|" + std::to_string((int)inputBuffer(cind)->axisState) + "," + std::to_string((int)inputBuffer(cind)->buttonStates) + "\n";
			++duration;
			if (duration >= motionComp.minDuration){
				if (firstInput){
					debugString += "---Last Input: Early Out\n";
					return 0;
				}
				if (motionComp.direction != 0){
					motionComp.direction = inputBuffer(cind)->axisState;
					motionComp.strict = true;
				}
			}
			--cind;
		}
		else{
			debugString += "--Invalid Frame " + std::to_string(cind) + " dur:" + std::to_string(duration) + "|" + std::to_string((int)inputBuffer(cind)->axisState) + "," + std::to_string((int)inputBuffer(cind)->buttonStates) + "\n";
			if (duration >= motionComp.minDuration){
				return glm::max(glm::abs(currentIndex - cind) - motionComp.minDuration + 1, 1);
			}
			else{
				duration = 0;
				--cind;
			}
		}
	} while (glm::abs(currentIndex - cind) - motionComp.minDuration <= maxBuffer);
	return -1;
}
bool InputDevice::InputMotionFrameCheck(InputMotionComponent* motionComp, int index){
	InputFrame* inpf = inputBuffer(index);//Current checked frame
	InputFrame* inpfp = inputBuffer(index - 1);//Current checked frame -1, to check for button presses

	if (motionComp->direction != 0){
		if ((inpf->axisState&motionComp->direction) == 0)
			return false;
		if (motionComp->strict && (inpf->axisState != motionComp->direction))
			return false;
	}
	for (auto i = motionComp->buttons.begin(); i != motionComp->buttons.end(); ++i){
		if (((int)(i->second) | (int)InputType::Pressed)){
			if ((inpf->buttonStates&i->first) && ((inpfp->buttonStates&i->first) == 0))
				continue;
		}
		if (((int)(i->second) | (int)InputType::Released)){
			if ((inpfp->buttonStates&i->first) && ((inpf->buttonStates&i->first) == 0))
				continue;
		}
		if (((int)(i->second) | (int)InputType::Held)){
			if ((inpf->buttonStates&i->first))
				continue;
		}
		return false;
	}
	return true;
}
InputFrame* InputDevice::inputBuffer(int index){
	if (index < 0){
		return &_inputBuffer[_inputBuffer.size()-1 - index%_inputBuffer.size()];
	}
	else
		return &_inputBuffer[index%_inputBuffer.size()];
}

void InputDevice::UpdateJoyInputs(){
	int count;
	const float* axes = glfwGetJoystickAxes(this->_deviceID,&count);
	this->cachedJoyAxes = std::vector<float>(axes,axes+count);
	const unsigned char* buttons = glfwGetJoystickButtons(this->_deviceID, &count);
	this->cachedJoyButtons = std::vector<unsigned char>(buttons, buttons + count);
}
void InputDevice::PollInput(){
	if (cachedInputFrames.size()==0||Time::lastUpdateTime + VE_FRAME_TIME*cachedInputFrames.size() < glfwGetTime()){
		cachedInputFrames.push_back(InputFrame(0,0));
	}

	if (this->_deviceID>-1 && !glfwJoystickPresent(this->_deviceID))
		return;

	glm::ivec2 dir;
	cachedInputFrames.back().axisState = 0;
	for (auto i = directionMap.begin(); i != directionMap.end(); ++i){
		if (EvaluateInput(i->second)){
			switch (i->first){
			case InputDirection::Down:
				dir.y -= 1;
				break;
			case InputDirection::Left:
				dir.x -= 1;
				break;
			case InputDirection::Right:
				dir.x += 1;
				break;
			case InputDirection::Up:
				dir.y += 1;
				break;
			}
		}
	}
	
	if (dir.x>0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Right);
	else if (dir.x<0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Left);
	if (dir.y>0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Up);
	else if (dir.y<0)
		cachedInputFrames.back().axisState |= (unsigned char)(InputDirection::Down);

	for (auto i = buttonMap.begin(); i != buttonMap.end(); ++i){
		if (EvaluateInput(i->second)){
			cachedInputFrames.back().buttonStates |= (unsigned char)(i->first);
		}
		//else
		//	cachedInputFrames.back().buttonStates &= ~((unsigned char)(i->first));
	}
	
}
void InputDevice::PushInputsToBuffer(){
	for (unsigned int i = 0; i < cachedInputFrames.size(); ++i){
		_inputBuffer[(this->_bufferEnd % 1200)] = cachedInputFrames[i];
		this->_bufferEnd = 1200 + ((this->_bufferEnd + 1) % 1200);
	}
	cachedInputFrames.clear();
}
std::string InputDevice::Serialize(){
	std::string str = "#BUTTONS\n";
	for (auto i = this->buttonMap.begin(); i != this->buttonMap.end(); ++i)
		str += std::to_string((int)i->first) + ":" + (i->second.isAxis ? "t," : "f,") + std::to_string(i->second.inputID) + "," + std::to_string((int)glm::sign(i->second.inputValue)) + "\n";
	str += "#AXES\n";
	for (auto i = this->directionMap.begin(); i != this->directionMap.end(); ++i)
		str += std::to_string((int)i->first) + ":" + (i->second.isAxis ? "t," : "f,") + std::to_string(i->second.inputID) + "," + std::to_string((int)glm::sign(i->second.inputValue)) + "\n";
	return str;
}
InputFrame* InputDevice::LastBufferInput(){
	return inputBuffer(this->_bufferEnd - 1);
}
std::string InputDevice::deviceName(){
	return _deviceName;
}
int InputDevice::deviceID(){
	return _deviceID;
}