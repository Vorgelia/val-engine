#include "Camera.h"
#include <GLM/gtc/matrix_transform.hpp>
#include "GameInstance.h"
#include "GameScene.h"
#include "Renderer.h"
#include "RenderingGL.h"
#include "GraphicsGL.h"
#include "ScreenManager.h"
#include "GameInstance.h"
#include "FrameBuffer.h"

VE_OBJECT_DEFINITION(BaseCamera);
VE_OBJECT_DEFINITION(OrthoCamera);

std::vector<RenderingCommand> BaseCamera::GatherRenderingCommands() const
{
	std::vector<RenderingCommand> renderingCommands;
	if(owningScene() == nullptr)
	{
		return renderingCommands;
	}

	const std::vector<ObjectReference<BaseRenderer>> renderers = owningScene()->FindObjectsOfType<BaseRenderer>();
	for(auto& iter : renderers)
	{
		if(iter.get() == nullptr)
		{
			continue;
		}

		std::vector<RenderingCommand> currentRenderingCommands = iter->GetRenderingCommands();
		if(currentRenderingCommands.empty())
		{
			continue;
		}

		renderingCommands.reserve(renderingCommands.size() + currentRenderingCommands.size());
		std::move(currentRenderingCommands.begin(), currentRenderingCommands.end(), renderingCommands.end());
	}

	return std::move(renderingCommands);
}

void BaseCamera::OnInit()
{
	ObjectComponent::OnInit();

	//TODO: Customization over framebuffer creation in BaseCamera::Serialize 
	_graphics = _owningInstance->Graphics();
	_frameBuffer = _graphics->CreateFrameBuffer(_owningInstance->ScreenManager()->size, _owningInstance->configData().renderingConfigData.frameBuferTextureAmount);

	_rendering = _owningInstance->Rendering();
	_rendering->RegisterCamera(this);
}

void BaseCamera::OnDestroyed()
{
	ObjectComponent::OnDestroyed();

	if(_graphics != nullptr)
	{
		_graphics->DestroyFrameBuffer(*_frameBuffer);
	}

	if(_rendering != nullptr)
	{
		_rendering->UnregisterCamera(this);
	}
}

void BaseCamera::HandleScreenResized()
{
	//_frameBuffer->resolution;
}

glm::mat4 BaseCamera::GetViewMatrix() const
{
	return GetWorldTransform().GetInverse().GetMatrix();
}

//------//

void OrthoCamera::RegisterReflectionFields() const
{
	AddReflectionField(VE_PRIVATE_REFLECTION_ARG(renderingScale));
	AddReflectionField(VE_PRIVATE_REFLECTION_ARG(zoomLevel));
}

void OrthoCamera::Deserialize(const json& j)
{
	BaseCamera::Deserialize(j);
	if(!TryDeserializeField("renderingScale", j))
	{
		_renderingScale = _owningInstance->configData().renderingConfigData.spriteRenderingScale;
	}
}

glm::mat4 OrthoCamera::GetProjectionMatrix() const
{
	const glm::vec2 zoomedRenderingScale = glm::vec2(_renderingScale) * _zoomLevel;
	return glm::ortho(float(-zoomedRenderingScale.x), float(zoomedRenderingScale.x), 0.0f, float(zoomedRenderingScale.y), 0.0f, 1.0f);
}

OrthoCamera::OrthoCamera()
	: _renderingScale(-1.0f, -1.0f)
	, _zoomLevel(1.0f)
{

}