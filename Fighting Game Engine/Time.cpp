#include "Time.h"
namespace Time{
	double deltaTime;
	double smoothDeltaTime;
	double time;
	double lastTime;
	double lastUpdateTime;
	double timeSinceLoad;
	float updateRate;
	unsigned long frameCount;
}
void Time::Update(){
	Time::lastTime = Time::time;
	Time::time = glfwGetTime();
	Time::deltaTime = Time::time - Time::lastTime;
	Time::smoothDeltaTime = std::max(Time::smoothDeltaTime + (Time::deltaTime - Time::smoothDeltaTime)*0.01,Time::deltaTime);
	updateRate = std::max(updateRate + (Time::deltaTime*VE_FRAME_RATE - updateRate)*0.01, Time::deltaTime*VE_FRAME_RATE);
	timeSinceLoad += deltaTime;
}

void Time::FrameUpdate(){
	++frameCount;
}