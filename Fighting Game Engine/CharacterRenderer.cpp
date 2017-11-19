#include "CharacterRenderer.h"
#include "GameCharacter.h"
#include "CharacterFrame.h"
#include "CharacterSprite.h"
#include "CharacterStateManager.h"
#include "Object.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "Rendering.h"

VE_BEHAVIOUR_REGISTER_TYPE(CharacterRenderer);

void CharacterRenderer::OnRenderObjects()
{
	if(_owner == nullptr || _owner->transform() == nullptr)
	{
		this->enabled = false;
		return;
	}

	if(_character == nullptr)
	{
		_character = _owner->GetBehaviour<GameCharacter>("GameCharacter");
		if(_character == nullptr)
		{
			this->enabled = false;
			return;
		}
	}

	if(_character->_stateManager->_currentFrame == nullptr)
	{
		Renderer::OnRenderObjects();
		return;
	}

	const CharacterSprite* spriteData = _character->_stateManager->_currentFrame->spriteData();
	Texture* frameTex = Resource::GetTexture(spriteData->sprite());

	HandleRenderingMaterial(spriteData, frameTex, _character->flipped());
	HandleRenderingTransform(spriteData, frameTex);

	Rendering::DrawMesh(_renderingTransform.get(), _mesh, _material);
}

void CharacterRenderer::HandleRenderingMaterial(const CharacterSprite* spriteData, Texture* texture, bool flipped)
{
	if(texture == nullptr)
	{
		return;
	}

	glm::ivec4 pixelRect = spriteData->pixelRect();

	glm::vec4 params(
		pixelRect.x * texture->size.z + (flipped ? pixelRect.z * texture->size.z : 0),
		pixelRect.y * texture->size.w,
		pixelRect.z * texture->size.z * (flipped ? -1 : 1),
		pixelRect.w * texture->size.w
	);

	_material->uniformTextures.insert_or_assign("tex0", MaterialTexture(texture, params));
}

void CharacterRenderer::HandleRenderingTransform(const CharacterSprite* spriteData, Texture* texture)
{
	glm::ivec2 originOffset = spriteData->originOffset();
	glm::vec2 sizeMultiplier = _character->sizeMultiplier();
	glm::ivec4 pixelRect = spriteData->pixelRect();

	_renderingTransform->SnapTo(*_owner->transform());

	_renderingTransform->position += glm::ivec2(
		glm::round(originOffset.x * sizeMultiplier.x),
		glm::round(originOffset.y * sizeMultiplier.y)
	);

	_renderingTransform->scale = glm::vec2(
		pixelRect.z * sizeMultiplier.x,
		pixelRect.w * sizeMultiplier.y);
}

CharacterRenderer::CharacterRenderer(Object* owner, const json& j) : Renderer(owner, j)
{
	if(_material == nullptr)
	{
		this->enabled = false;
		return;
	}

	_material = Resource::CopyMaterial(_material);
	_renderingTransform = std::make_unique<Transform>(_owner);
}

CharacterRenderer::~CharacterRenderer()
{
}
