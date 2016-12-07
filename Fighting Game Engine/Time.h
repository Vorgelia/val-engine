#pragma once
#include "CommonUtilIncludes.hpp"
#define VE_FRAME_TIME 0.0166666666
#define VE_FRAME_RATE 60
namespace Time{
	extern double deltaTime;
	extern double smoothDeltaTime;
	extern double time;
	extern double lastTime;
	extern double lastUpdateTime;
	extern double timeSinceLoad;
	extern float updateRate;
	extern unsigned long frameCount;
	void Update();
	void FrameUpdate();
}

