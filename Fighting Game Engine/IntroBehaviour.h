#pragma once
#include "Behaviour.h"

class GameSceneManager;
class InputManager;
class RenderingGL;
class Time;
class ResourceManager;

class IntroBehaviour :
	public Behaviour
{
private:
	GameSceneManager* _gameSceneManager;
	InputManager* _input;
	RenderingGL* _rendering;
	Time* _time;
	Debug* _debug;
	ResourceManager* _resource;

private:
	float _introDuration = 4;

public:
	VE_BEHAVIOUR_NAME(IntroBehaviour);

	VE_BEHAVIOUR_REGISTER_FUNCTION(OnSceneInit);
	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderUI);
	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);

	IntroBehaviour(Object* owner, GameInstance* serviceManager);
	IntroBehaviour(Object* owner, GameInstance* serviceManager, const json& j);
	~IntroBehaviour() = default;
};

