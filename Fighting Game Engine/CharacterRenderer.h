#pragma once
#include "Renderer.h"
#include <memory>

class GameCharacter;
struct Transform;
class Texture;
class CharacterSprite;
class Material;

class ResourceManager;
class RenderingGL;

class CharacterRenderer :
	public Renderer
{
	VE_OBJECT_DECLARATION(CharacterRenderer);

private:
	ResourceManager* _resource;
	RenderingGL* _rendering;

private:
	ObjectReference<GameCharacter> _character;
	std::unique_ptr<Material> _renderingMaterialCopy;

	glm::vec2 _pixelsToWorldUnits;

	void HandleRenderingMaterial(const CharacterSprite* spriteData, Texture* texture, bool flipped) const;
	Transform GetRenderingTransform(const CharacterSprite* spriteData, Texture* texture) const;

	virtual std::vector<RenderingCommand> GetRenderingCommands(const BaseCamera* camera) const override;

public:

	virtual void OnInit() override;
	virtual void OnDestroyed() override;

	CharacterRenderer() = default;
	~CharacterRenderer() = default;
};