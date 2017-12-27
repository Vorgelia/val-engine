#include "CharacterRenderer.h"
#include <GLM\glm.hpp>
#include "GameCharacter.h"
#include "CharacterFrame.h"
#include "CharacterSprite.h"
#include "CharacterStateManager.h"
#include "ServiceManager.h"
#include "Object.h"
#include "Material.h"
#include "Texture.h"
#include "Transform.h"
#include "RenderingGL.h"
#include "ResourceManager.h"

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
	Texture* frameTex = _resource->GetTexture(spriteData->sprite());

	HandleRenderingMaterial(spriteData, frameTex, _character->_flipped);
	HandleRenderingTransform(spriteData, frameTex);

	_rendering->DrawMesh(_renderingTransform.get(), _mesh, _material);
}

void CharacterRenderer::HandleRenderingMaterial(const CharacterSprite* spriteData, Texture* texture, bool flipped)
{
	if(texture == nullptr)
	{
		return;
	}

	glm::ivec4 pixelRect = spriteData->pixelRect();
	glm::vec4 textureSize = texture->size();

	glm::vec4 params(
		pixelRect.x * textureSize.z + (flipped ? pixelRect.z * textureSize.z : 0),
		pixelRect.y * textureSize.w,
		pixelRect.z * textureSize.z * (flipped ? -1 : 1),
		pixelRect.w * textureSize.w
	);

	_material->uniformTextures.insert_or_assign("tex0", MaterialTexture(texture, params));
}

void CharacterRenderer::HandleRenderingTransform(const CharacterSprite* spriteData, Texture* texture)
{
	glm::ivec2 originOffset = spriteData->originOffset();
	glm::vec2 sizeMultiplier = _character->_sizeMultiplier;
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

CharacterRenderer::CharacterRenderer(Object* owner, ServiceManager* serviceManager, const json& j) : Renderer(owner, serviceManager, j)
{
	if(_material == nullptr)
	{
		this->enabled = false;
		return;
	}

	_resource = _serviceManager->ResourceManager();
	_rendering = _serviceManager->Rendering();

	_material = _resource->CopyMaterial(_material);
	_renderingTransform = std::make_unique<Transform>(_owner, serviceManager);
}

CharacterRenderer::~CharacterRenderer()
{
}
