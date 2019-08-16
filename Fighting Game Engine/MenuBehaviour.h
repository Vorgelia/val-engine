#pragma once
#include "ObjectComponent.h"

struct MaterialTexture;

class GameInstance;
class Time;
class RenderingGL;
class InputManager;
class ResourceManager;
class ScreenManager;

class MenuBehaviour : public ObjectComponent
{
	VE_OBJECT_DECLARATION(MenuBehaviour, ObjectComponent);

private:
	ScreenManager* _screen;
	InputManager* _input;
	RenderingGL* _rendering;
	ResourceManager* _resource;

private:
	float _motionTimer;
	std::unique_ptr<MaterialTexture> _fadingOverlay;

public:
	void OnInit() override;

	void OnGameUpdate();
	void OnRenderUI();

	MenuBehaviour() = default;
	~MenuBehaviour() = default;
};

