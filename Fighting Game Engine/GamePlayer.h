#pragma once
#include <string>

class ServiceManager;
class InputManager;
class InputDevice;
enum class InputDeviceId;

enum class GamePlayerType
{
	None = 0,
	Player = 1,
	Spectator = 2
};

class GamePlayer
{
	friend class GameCharacter;
	friend class PlayerManager;

protected:
	InputManager* _input;

protected:
	int _id;
	std::string _name;
	GamePlayerType _type;

	InputDevice* _inputDevice;

	virtual void Update();

	void HandleDeviceRemoved(InputDevice* device);

public:
	void SetDevice(int deviceId);

	GamePlayer(GamePlayerType type, ServiceManager* serviceManager, int deviceId);
	~GamePlayer();
};