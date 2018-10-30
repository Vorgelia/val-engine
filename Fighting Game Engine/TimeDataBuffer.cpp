#include "TimeDataBuffer.h"

void TimeDataBuffer::SetupData(float time, float deltaTime)
{
	_data.resize(8, 0.0f);

	_data[0] = time;
	_data[1] = deltaTime;
	_data[2] = 1.0f / time;
	_data[3] = 1.0f / deltaTime;
	_data[4] = time * _data[3];
	_data[5] = deltaTime * _data[2];
	_data[6] = 0.0f;
	_data[7] = 0.0f;
}