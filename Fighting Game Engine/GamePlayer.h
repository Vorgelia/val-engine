#pragma once

class InputDevice;

class GamePlayer
{
	friend class GameCharacter;

	InputDevice* _currentDevice;

public:
	void SetDeviceId(int deviceId);

	GamePlayer();
	~GamePlayer();
};