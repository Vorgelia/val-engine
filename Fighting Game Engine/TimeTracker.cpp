#include "TimeTracker.h"

void TimeTracker::Reset(double currentTime)
{
	_timeOffset = currentTime;
	_time = currentTime;
	_deltaTime = 0.0f;
}

void TimeTracker::Update(double currentTime)
{
	_deltaTime = currentTime - _time;
	_time = currentTime;
}

void TimeTracker::Update(const TimeTracker& baseTimeTracker)
{
	_deltaTime = baseTimeTracker.deltaTime();
	_time = baseTimeTracker.time();
}

TimeTracker::TimeTracker()
	: _timeOffset(0.0)
	, _time(0.0)
	, _deltaTime(0.0)
{
	
}
