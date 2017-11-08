#pragma once
#include "Renderer.h"

class GameCharacter;

class CharacterRenderer :
	public Renderer
{
	GameCharacter* _character;

public:
	VE_BEHAVIOUR_NAME(CharacterRenderer);

	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderObjects);

	CharacterRenderer(const json& j);
	virtual ~CharacterRenderer();
};

