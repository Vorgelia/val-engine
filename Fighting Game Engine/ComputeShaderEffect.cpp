#include "ComputeShaderEffect.h"
#include "ServiceManager.h"
#include "GraphicsGL.h"
#include "Screen.h"
#include "ResourceManager.h"

VE_BEHAVIOUR_REGISTER_TYPE(ComputeShaderEffect);

void ComputeShaderEffect::OnApplyPostEffects()
{
	if(_computeShader == nullptr)
	{
		return;
	}

	_graphics->DispatchCompute(*_computeShader, 1, 1);
}

ComputeShaderEffect::ComputeShaderEffect(Object* owner, ServiceManager* serviceManager, const json& j)
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
