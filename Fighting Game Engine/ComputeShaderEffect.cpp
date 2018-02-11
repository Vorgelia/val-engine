#include "ComputeShaderEffect.h"
#include "ServiceManager.h"
#include "GraphicsGL.h"
#include "RenderingGL.h"
#include "Screen.h"
#include "FrameBuffer.h"
#include "Texture.h"
#include "ResourceManager.h"

VE_BEHAVIOUR_REGISTER_TYPE(ComputeShaderEffect);

void ComputeShaderEffect::OnApplyPostEffects()
{
	if(_computeShader == nullptr)
	{
		return;
	}

	_graphics->DispatchCompute(*_computeShader, _serviceManager->Screen()->size.x, _serviceManager->Screen()->size.y);
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
{
}
