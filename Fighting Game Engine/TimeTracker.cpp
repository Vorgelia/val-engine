#include "Time.h"

void TimeTracker::Reset(double currentTime)
{
	_timeOffset = currentTime;
	_time = currentTime;
	_deltaTime = 0.0f;
}

void TimeTracker::Update(double currentTime)
{
	_deltaTime = currentTime - _timeOffset;
	_time = currentTime;
}

TimeTracker::TimeTracker()
	: _timeOffset(0.0)
	, _time(0.0)
	, _deltaTime(0.0)
{
	
}
