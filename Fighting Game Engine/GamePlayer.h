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
	std::string _id;
	GamePlayerType _type;

	InputDevice* _inputDevice;

	virtual void Update();

public:

	GamePlayer(GamePlayerType type, int deviceId);
	~GamePlayer();
};