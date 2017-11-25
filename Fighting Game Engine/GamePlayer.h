#pragma once
#include <string>

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
	int _id;
	std::string _name;
	GamePlayerType _type;

	InputDevice* _inputDevice;

	virtual void Update();

	void HandleDeviceRemoved(InputDevice* device);

public:
	void SetDevice(int deviceId);

	GamePlayer(GamePlayerType type, int deviceId);
	~GamePlayer();
};