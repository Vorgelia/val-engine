#include "Time.h"
#include <Windows.h>
#include "GLIncludes.hpp"
#include "MathIncludes.hpp"

void Time::Init()
{

}

void Time::Update()
{
	lastTime = time;
	time = glfwGetTime();
	deltaTime = time - lastTime;
	smoothDeltaTime = glm::max<double>(glm::lerp<double>(smoothDeltaTime, deltaTime, deltaTime * 3), deltaTime);
	smoothUpdateRate = glm::max<double>(glm::lerp<double>(smoothUpdateRate, deltaTime * VE_FRAME_RATE, deltaTime * 3), deltaTime * VE_FRAME_RATE);
	timeSinceLoad += deltaTime;
	updateRate = 0;
}

void Time::Cleanup()
{
}

//Advance game update related variables by one frame
void Time::FrameUpdate()
{
	updateRate += 1;
	frameCount += 1;
	frameCountSinceLoad += 1;
	lastUpdateTime += VE_FRAME_TIME;
}

void Time::HandleSceneLoaded()
{
	timeSinceLoad = 0;
	lastUpdateTime = time - VE_FRAME_TIME;
	frameCountSinceLoad = 0;
	smoothUpdateRate = 0;
}

Time::Time(ServiceManager* serviceManager) : BaseService(serviceManager, -100)
{
	_allowServiceUpdate = true;
}