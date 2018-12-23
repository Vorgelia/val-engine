#pragma once

class TimeTracker
{
protected:
	double _timeOffset;

	double _time;
	double _deltaTime;

public:
	double time() const { return _time; }
	double deltaTime() const { return _deltaTime; }

	void Reset(double currentTime);
	void Update(double currentTime);

	TimeTracker();
	~TimeTracker() = default;
};