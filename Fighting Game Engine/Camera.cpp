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

		renderingCommands.insert(renderingCommands.end(), std::make_move_iterator(currentRenderingCommands.begin()), std::make_move_iterator(currentRenderingCommands.end()));
	}

	return std::move(renderingCommands);
}

FrameBuffer* BaseCamera::targetFrameBuffer() const
{
	return _targetFrameBuffer == nullptr ? _rendering->mainBuffer() : targetFrameBuffer();
}

void BaseCamera::OnInit()
{
	ObjectComponent::OnInit();

	//TODO: Customization over framebuffer creation in BaseCamera::Serialize 
	_graphics = _owningInstance->Graphics();

	_rendering = _owningInstance->Rendering();
	_rendering->RegisterCamera(this);
}

void BaseCamera::OnDestroyed()
{
	ObjectComponent::OnDestroyed();

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
	return glm::inverse(GetWorldTransform().GetMatrix());
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
	if(_renderingScale.x <= 0 || _renderingScale.y <= 0)
	{
		_renderingScale = _owningInstance->configData().renderingConfigData.worldRenderingScale;
	}
}

glm::mat4 OrthoCamera::GetProjectionMatrix() const
{
	const glm::vec2 zoomedRenderingScale = glm::vec2(_renderingScale) * _zoomLevel;
	return glm::ortho(float(-zoomedRenderingScale.x) * 0.5f, float(zoomedRenderingScale.x) * 0.5f, 0.0f, float(zoomedRenderingScale.y), 100.0f, -100.0f);
}

OrthoCamera::OrthoCamera()
	: _renderingScale(-1.0f, -1.0f)
	, _zoomLevel(1.0f)
{

}