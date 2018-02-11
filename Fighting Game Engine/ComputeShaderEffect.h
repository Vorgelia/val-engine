#pragma once
#include "Behaviour.h"

class GraphicsGL;
class RenderingGL;
class ResourceManager;
class Shader;

class ComputeShaderEffect :
	public Behaviour
{
private:
	GraphicsGL* _graphics;
	RenderingGL* _rendering;
	ResourceManager* _resource;

private:
	Shader* _computeShader;

public:
	VE_BEHAVIOUR_NAME(ComputeShaderEffect);

	VE_BEHAVIOUR_REGISTER_FUNCTION(OnApplyPostEffects);

	ComputeShaderEffect(Object* owner, ServiceManager* serviceManager, const json& j);
	~ComputeShaderEffect();
};

