#pragma once
#include "Behaviour.h"

struct MaterialTexture;

class ServiceManager;
class Time;
class RenderingGL;
class InputManager;
class ResourceManager;
class Screen;

class MenuBehaviour :
	public Behaviour
{
private:
	Time* _time;
	Screen* _screen;
	InputManager* _input;
	RenderingGL* _rendering;
	ResourceManager* _resource;

private:
	float _motionTimer;
	std::unique_ptr<MaterialTexture> _fadingOverlay;

public:
	VE_BEHAVIOUR_NAME(MenuBehaviour);

	VE_BEHAVIOUR_REGISTER_FUNCTION(EngineUpdate);
	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);
	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderUI);

	MenuBehaviour(Object* owner, ServiceManager* serviceManager, const json& j);
	~MenuBehaviour();
};

