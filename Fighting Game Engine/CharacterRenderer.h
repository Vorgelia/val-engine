#pragma once
#include "Renderer.h"
#include <memory>

class GameCharacter;
class Transform;
class Texture;
class CharacterSprite;

class CharacterRenderer :
	public Renderer
{
	GameCharacter* _character;
	std::unique_ptr<Transform> _renderingTransform;

	glm::vec2 _pixelsToWorldUnits;

	void HandleRenderingMaterial(const CharacterSprite* spriteData, Texture* texture, bool flipped);
	void HandleRenderingTransform(const CharacterSprite* spriteData, Texture* texture);

public:
	VE_BEHAVIOUR_NAME(CharacterRenderer);

	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderObjects);

	CharacterRenderer(Object* owner, const json& j);
	virtual ~CharacterRenderer();
};