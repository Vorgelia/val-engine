#pragma once
#include <GLM/glm.hpp>
#include "ObjectComponent.h"

class FrameBuffer;
class RenderingGL;
class GraphicsGL;

class RenderingCommand;

class BaseCamera : public ObjectComponent
{
	friend class RenderingGL;

protected:
	ObjectReference<RenderingGL> _rendering;
	ObjectReference<GraphicsGL> _graphics;

	FrameBuffer* _targetFrameBuffer;

	std::vector<RenderingCommand> GatherRenderingCommands() const;

public:
	int renderingPriority;

	const FrameBuffer* targetFrameBuffer() const;

	void OnInit() override;
	void OnDestroyed() override;

	virtual void HandleScreenResized();

	virtual glm::mat4 GetProjectionMatrix() const = 0;
	virtual glm::mat4 GetViewMatrix() const;
	BaseCamera() = default;
};

class OrthoCamera : public BaseCamera
{
	VE_OBJECT_DECLARATION(OrthoCamera);

protected:
	ve::vec2 _renderingScale;
	float _zoomLevel;

public:
	const ve::vec2& renderingScale() const { return _renderingScale; }
	float zoomLevel() const { return _zoomLevel; }

	void SetRenderingScale(ve::vec2 renderingScale) { _renderingScale = std::move(renderingScale); }
	void SetZoomLevel(float zoomLevel) { _zoomLevel = zoomLevel; }

	virtual glm::mat4 GetProjectionMatrix() const override;

	virtual void RegisterReflectionFields() const override;
	virtual void Deserialize(const json& j) override;

	OrthoCamera();
};