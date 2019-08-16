#pragma once
#include "ObjectComponent.h"

class GraphicsGL;
class RenderingGL;
class ResourceManager;
class ComputeShader;
class BaseCamera;

class ComputeShaderEffect :	public ObjectComponent
{
	VE_OBJECT_DECLARATION(ComputeShaderEffect, ObjectComponent);

protected:
	GraphicsGL* _graphics;
	RenderingGL* _rendering;
	ResourceManager* _resource;

protected:
	ComputeShader* _computeShader;
	ObjectReference<BaseCamera> _sceneCamera;

	ve::vec3 _lastCameraPos;
	ve::vec3 _cameraUVDelta;

	void OnDeserialized(BaseSerializationProxy& proxy) override;

public:
	void OnInit() override;
	void OnDestroyed() override;

	void UpdateEffect();
};

