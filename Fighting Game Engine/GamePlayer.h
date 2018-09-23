#pragma once
#include <string>

class ServiceManager;
class InputManager;
class InputDevice;
enum class InputDeviceId;

class GamePlayer
{
	friend class GameCharacter;
	friend class PlayerManager;

protected:
	InputManager* _input;

protected:
	int _id;
	std::string _name;

	InputDevice* _inputDevice;

	virtual void Update();

	void HandleDeviceRemoved(InputDevice* device);

public:
	InputDevice* inputDevice() const;

	void SetDevice(int deviceId);

	GamePlayer(int id, int deviceId, ServiceManager* serviceManager);
	virtual ~GamePlayer();
};