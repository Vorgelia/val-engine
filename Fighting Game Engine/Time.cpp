#include "Time.h"
#include <windows.h>
#include "GLIncludes.hpp"
#include <GLM\glm.hpp>

//Variables related to keeping track of various time-related concepts.
namespace Time
{
	double deltaTime;
	double smoothDeltaTime;
	double time;
	double lastTime;
	double lastUpdateTime;
	double timeSinceLoad;
	float updateRate;
	unsigned long frameCount;
}

void Time::Update()
{
	Time::lastTime = Time::time;
	Time::time = glfwGetTime();
	Time::deltaTime = Time::time - Time::lastTime;
	Time::smoothDeltaTime = glm::max<double>(Time::smoothDeltaTime + (Time::deltaTime - Time::smoothDeltaTime)*0.01, Time::deltaTime);
	Time::updateRate = glm::max<double>(updateRate + (Time::deltaTime * VE_FRAME_RATE - updateRate)*0.01, Time::deltaTime * VE_FRAME_RATE);
	Time::timeSinceLoad += deltaTime;
}

//Advance game update related variables by one frame
void Time::FrameUpdate()
{
	frameCount += 1;
	Time::lastUpdateTime += VE_FRAME_TIME;
}