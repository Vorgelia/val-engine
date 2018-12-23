#include "CharacterRenderer.h"
#include <GLM/glm.hpp>
#include "GameCharacter.h"
#include "CharacterFrame.h"
#include "CharacterSprite.h"
#include "CharacterStateComponent.h"
#include "GameInstance.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "GameCharacterData.h"

VE_OBJECT_DEFINITION(CharacterRenderer);

void CharacterRenderer::HandleRenderingMaterial(const CharacterSprite* spriteData, Texture* texture, bool flipped) const
{
	if(texture == nullptr)
	{
		return;
	}

	const ve::ivec4& pixelRect = spriteData->pixelRect();
	const glm::vec4& textureSize = texture->size();

	glm::vec4 params(
		pixelRect.x * textureSize.z + (flipped ? pixelRect.z * textureSize.z : 0),
		pixelRect.y * textureSize.w,
		pixelRect.z * textureSize.z * (flipped ? -1 : 1),
		pixelRect.w * textureSize.w
	);

	_material->uniformTextures.insert_or_assign("tex0", MaterialTexture(texture, params));
}

Transform CharacterRenderer::GetRenderingTransform(const CharacterSprite* spriteData, Texture* texture) const
{
	ve::ivec2 originOffset = spriteData->originOffset();
	ve::vec2 sizeMultiplier = _character->characterData()->_sizeMultiplier;
	ve::ivec4 pixelRect = spriteData->pixelRect();

	Transform outTransform = _owner->GetWorldTransform();

	outTransform.SetPosition(outTransform.GetPosition() + ve::vec3(
		ve::dec_t(originOffset.x) * sizeMultiplier.x,
		ve::dec_t(originOffset.y) * sizeMultiplier.y,
		0
	));

	outTransform.SetScale(ve::vec3(
		ve::dec_t(pixelRect.z) * sizeMultiplier.x,
		ve::dec_t(pixelRect.w) * sizeMultiplier.y,
		0)
	);

	return outTransform;
}

std::vector<RenderingCommand> CharacterRenderer::GetRenderingCommands() const
{
	if(!_character.IsValid() || _character->_stateComponent->_currentFrame == nullptr)
	{
		return std::vector<RenderingCommand>();
	}

	const CharacterSprite* spriteData = _character->_stateComponent->_currentFrame->spriteData();
	Texture* frameTex = _resource->GetTexture(spriteData->sprite());

	const Transform renderingTransform = GetRenderingTransform(spriteData, frameTex);
	HandleRenderingMaterial(spriteData, frameTex, _character->_flipped);

	std::vector<RenderingCommand> renderingCommands{ RenderingCommand(_mesh, renderingTransform, _material) };
	return renderingCommands;
}

void CharacterRenderer::OnInit()
{
	if(_material == nullptr)
	{
		this->enabled = false;
		return;
	}

	_resource = _owningInstance->ResourceManager();
	_rendering = _owningInstance->Rendering();

	_renderingMaterialCopy = std::make_unique<Material>(*_material);
	_material = _renderingMaterialCopy.get();

	_character = dynamic_cast<GameCharacter*>(_owner.get());
}
