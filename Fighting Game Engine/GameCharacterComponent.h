#pragma once

class GameInstance;
class GameCharacter;

class GameCharacterComponent
{
	friend class GameCharacter;

protected:
	GameInstance* _serviceManager;
	GameCharacter* _owner;

protected:
	virtual void Init() {}
	virtual void Update() {}
	virtual void LateUpdate() {}

public:
	GameCharacterComponent(GameCharacter* owner, GameInstance* serviceManager);
	virtual ~GameCharacterComponent() = default;
};

