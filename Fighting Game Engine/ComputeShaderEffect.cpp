#include "ComputeShaderEffect.h"
#include "GameInstance.h"
#include "GraphicsGL.h"
#include "ScreenManager.h"
#include "ResourceManager.h"
#include "RenderingGL.h"
#include "Camera.h"

VE_OBJECT_DEFINITION(ComputeShaderEffect);

void ComputeShaderEffect::UpdateEffect()
{
	if(!_sceneCamera.IsValid() || _computeShader == nullptr)
	{
		return;
	}

	const Transform& sceneCameraTransform = _sceneCamera->GetWorldTransform();
	_cameraUVDelta = sceneCameraTransform.GetPosition() - _lastCameraPos;
	_lastCameraPos = sceneCameraTransform.GetPosition();

	_graphics->BindShader(*_computeShader);
	glUniform2f(_computeShader->UniformLocation("uvOffset"), float{ _cameraUVDelta.x }, float{ _cameraUVDelta.y });
	_graphics->DispatchCompute(*_computeShader, 1, 1);
}

void ComputeShaderEffect::Deserialize(const json& j)
{
	_resource = _owningInstance->ResourceManager();
	_graphics = _owningInstance->Graphics();
	_rendering = _owningInstance->Rendering();

	std::string shaderPath;
	if(JSON::TryGetMember(j, "shaderPath", shaderPath))
	{
		_computeShader = _resource->GetComputeShader(shaderPath);
	}
}

void ComputeShaderEffect::OnInit()
{
	 _sceneCamera = _owningInstance->GameSceneManager()->currentScene()->FindObjectOfType<BaseCamera>();
	 if(_sceneCamera.IsValid())
	 {
		 _lastCameraPos = _sceneCamera->GetWorldTransform().GetPosition();
	 }

	VE_REGISTER_UPDATE_FUNCTION(UpdateGroup::PostProcess, UpdateType::LastFixedGameUpdate, UpdateEffect);
}

void ComputeShaderEffect::OnDestroyed()
{
	
}
