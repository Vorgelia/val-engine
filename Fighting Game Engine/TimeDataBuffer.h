#pragma once
#include "GraphicsBuffer.h"

class TimeDataBuffer :
	public GraphicsBuffer
{
public:
	void SetupData(float time, float deltaTime, float frameCount);

	TimeDataBuffer();
	~TimeDataBuffer();
};

