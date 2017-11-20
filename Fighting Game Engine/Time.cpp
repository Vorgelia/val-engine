#include "Time.h"
#include <windows.h>
#include "GLIncludes.hpp"
#include "MathIncludes.hpp"

//Time-keeping variables
namespace Time
{
	double deltaTime;
	double smoothDeltaTime;
	double time;
	double lastTime;
	double lastUpdateTime;
	double timeSinceLoad;
	double updateRate;
	unsigned long long frameCount;
	unsigned long long frameCountSinceLoad;
}

void Time::Update()
{
	Time::lastTime = Time::time;
	Time::time = glfwGetTime();
	Time::deltaTime = Time::time - Time::lastTime;
	Time::smoothDeltaTime = glm::max<double>(glm::lerp<double>(Time::smoothDeltaTime, Time::deltaTime, Time::deltaTime * 3), Time::deltaTime);
	Time::updateRate = glm::max<double>(glm::lerp<double>(Time::updateRate, Time::deltaTime * VE_FRAME_RATE, Time::deltaTime * 3), Time::deltaTime * VE_FRAME_RATE);
	Time::timeSinceLoad += deltaTime;
}

//Advance game update related variables by one frame
void Time::FrameUpdate()
{
	Time::frameCount += 1;
	Time::frameCountSinceLoad += 1;
	Time::lastUpdateTime += VE_FRAME_TIME;
}

void Time::OnSceneLoaded()
{
	Time::timeSinceLoad = 0;
	Time::lastUpdateTime = Time::time - VE_FRAME_TIME;
	Time::frameCountSinceLoad = 0;
	Time::updateRate = 0;
}