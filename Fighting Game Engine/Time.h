#pragma once

#define VE_FRAME_TIME 0.0166666666
#define VE_FRAME_RATE 60
namespace Time
{
	extern double deltaTime;
	extern double smoothDeltaTime;
	extern double time;
	extern double lastTime;
	extern double lastUpdateTime;
	extern double timeSinceLoad;
	extern double updateRate;
	extern unsigned long long frameCount;
	extern unsigned long long frameCountSinceLoad;

	void Update();
	void FrameUpdate();
	void OnSceneLoaded();
}

