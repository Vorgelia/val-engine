#pragma once

class ServiceManager;
class GameCharacter;

class GameCharacterComponent
{
	friend class GameCharacter;

protected:
	ServiceManager* _serviceManager;
	GameCharacter* _owner;

protected:
	virtual void Init() {}
	virtual void Update() {}

public:
	GameCharacterComponent(GameCharacter* owner, ServiceManager* serviceManager);
	virtual ~GameCharacterComponent() = default;
};

