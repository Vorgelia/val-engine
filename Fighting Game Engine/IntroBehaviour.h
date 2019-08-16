#pragma once
#include "BaseSceneBehavior.h"

class GameSceneManager;
class InputManager;
class RenderingGL;
class Debug;
class ResourceManager;

class IntroBehaviour : public BaseSceneBehavior
{
	VE_OBJECT_DECLARATION(IntroBehaviour, BaseSceneBehavior)

private:
	GameSceneManager* _gameSceneManager;
	InputManager* _input;
	RenderingGL* _rendering;
	Debug* _debug;
	ResourceManager* _resource;

private:
	float _introDuration = 4;
	std::string _scriptPath;

public:
	virtual void RegisterReflectionFields() const;

	virtual void OnInit() override;

	void OnRenderUI();
	void OnGameUpdate();

	IntroBehaviour() = default;
	~IntroBehaviour() = default;
};

