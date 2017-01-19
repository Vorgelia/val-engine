#pragma once

class Object;
class GameCharacter
{
public:

	int currentState;
	int currentFrame;

	Object* characterObject;

	GameCharacter();
	~GameCharacter();
};