#include "TimeDataBuffer.h"

void TimeDataBuffer::SetupData(float time, float deltaTime, float frameCount)
{
	_data.resize(8, 0.0f);

	_data[0] = time;
	_data[1] = 1.0f / time;
	_data[2] = deltaTime;
	_data[3] = 1.0f / deltaTime;
	_data[4] = frameCount;
	_data[5] = 1.0f / frameCount;
	_data[6] = 0.0f;
	_data[7] = 0.0f;
}

TimeDataBuffer::TimeDataBuffer()
{
}


TimeDataBuffer::~TimeDataBuffer()
{
}
