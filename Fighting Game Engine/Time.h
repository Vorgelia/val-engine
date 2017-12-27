#pragma once
#include "BaseService.h"

#define VE_FRAME_RATE 60
#define VE_FRAME_TIME (1.0/VE_FRAME_RATE)

class Time : public BaseService
{
public:
	double deltaTime;
	double smoothDeltaTime;
	double time;
	double lastTime;
	double lastUpdateTime;
	double timeSinceLoad;
	double updateRate;
	unsigned long long frameCount;
	unsigned long long frameCountSinceLoad;

	void Init() override;
	void Update() override;

	void FrameUpdate();
	void HandleSceneLoaded();

	Time(ServiceManager* serviceManager);
	~Time() = default;
};