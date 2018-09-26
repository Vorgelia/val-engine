#include "ComputeShaderEffect.h"
#include "GameInstance.h"
#include "GraphicsGL.h"
#include "Screen.h"
#include "ResourceManager.h"
#include "RenderingGL.h"
#include "Camera.h"

VE_BEHAVIOUR_REGISTER_TYPE(ComputeShaderEffect);

void ComputeShaderEffect::LateGameUpdate()
{
	_cameraUVDelta = _rendering->cameras[0].position - _lastCameraPos;
	_lastCameraPos = _rendering->cameras[0].position;
}

void ComputeShaderEffect::OnApplyPostEffects()
{
	if(_computeShader == nullptr)
	{
		return;
	}

	_graphics->BindShader(*_computeShader);
	glUniform2f(_computeShader->UniformLocation("uvOffset"), _cameraUVDelta.x, _cameraUVDelta.y);
	_graphics->DispatchCompute(*_computeShader, 1, 1);
}

ComputeShaderEffect::ComputeShaderEffect(Object* owner, GameInstance* serviceManager, const json& j)
	: Behaviour(owner, serviceManager, j)
{
	_resource = serviceManager->ResourceManager();
	_graphics = serviceManager->Graphics();
	_rendering = serviceManager->Rendering();

	std::string shaderPath;
	if(JSON::TryGetMember(j, "shaderPath", shaderPath))
	{
		_computeShader = _resource->GetComputeShader(shaderPath);
	}
}

ComputeShaderEffect::~ComputeShaderEffect()
= default;
