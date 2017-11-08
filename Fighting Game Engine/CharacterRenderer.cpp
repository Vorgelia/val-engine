#include "CharacterRenderer.h"
#include "GameCharacter.h"
#include "CharacterFrame.h"
#include "CharacterSprite.h"
#include "Object.h"
#include "Material.h"

void CharacterRenderer::OnRenderObjects()
{
	if(_character == nullptr)
	{
		_character = _owner->GetBehaviour<GameCharacter>("GameCharacter");
		if(_character == nullptr)
		{
			this->enabled = false;
			return;
		}
	}

	 Texture* frameTex =  Resource::GetTexture(_character->currentFrame()->frameData()->sprite());
	 if(frameTex == nullptr)
	 {
		 return;
	 }

	 //glm::vec4 params;
	 //_material->uniformTextures["tex0"] = MaterialTexture(frameTex,);

	 Renderer::OnRenderObjects();
}

CharacterRenderer::CharacterRenderer(const json& j) : Renderer(j)
{
	if(_material == nullptr)
	{
		this->enabled = false;
		return;
	}

	_material = Resource::CopyMaterial(_material);
}


CharacterRenderer::~CharacterRenderer()
{
}
