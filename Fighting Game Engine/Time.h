#pragma once
#include "BaseService.h"
#include "ValEngine.h"

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

	int updateRate;
	double smoothUpdateRate;

	ve::int_t frameCount;
	ve::int_t frameCountSinceLoad;

	void Init() override;
	void Update() override;
	void Cleanup() override;

	void FrameUpdate();
	void HandleSceneLoaded();

	Time(GameInstance* serviceManager);
	~Time() = default;
};