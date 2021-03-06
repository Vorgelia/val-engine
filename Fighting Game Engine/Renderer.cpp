#include "Renderer.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "GameInstance.h"
#include <string>
#include "ObjectFactory.h"
#include "Mesh.h"

VE_OBJECT_DEFINITION(Renderer);
VE_OBJECT_DEFINITION(ParallaxRenderer);

void Renderer::OnDeserialized(BaseSerializationProxy& proxy)
{
	BaseRenderer::OnDeserialized(proxy);

	std::string resourcePath;
	if (proxy.Get("material", resourcePath))
	{
		_material = _owningInstance->ResourceManager()->GetMaterial(resourcePath);
	}
	if (proxy.Get("mesh", resourcePath))
	{
		_mesh = _owningInstance->ResourceManager()->GetMesh(resourcePath);
	}
}

std::vector<RenderingCommand> Renderer::GetRenderingCommands(const BaseCamera* camera) const
{
	return std::vector<RenderingCommand> { RenderingCommand(_mesh, GetWorldTransform(), _material) };
}

void ParallaxRenderer::RegisterReflectionFields() const
{
	VE_PRIVATE_REFLECTION_VAR(Field, parallaxScale);
}

std::vector<RenderingCommand> ParallaxRenderer::GetRenderingCommands(const BaseCamera* camera) const
{
	Transform modifiedWorldTransform = GetWorldTransform();

	ve::vec3 cameraDifferenceXY = camera->GetWorldTransform().GetPosition();
	cameraDifferenceXY.z = 0;

	if(modifiedWorldTransform.GetPosition().z >= 0.0f)
	{
		modifiedWorldTransform.SetPosition(modifiedWorldTransform.GetPosition() + cameraDifferenceXY * FixedPoint64(_parallaxScale) * (FixedPoint64::one - FixedPoint64::one / modifiedWorldTransform.GetPosition().z));
	}
	else
	{
		modifiedWorldTransform.SetPosition(modifiedWorldTransform.GetPosition() + cameraDifferenceXY * FixedPoint64(_parallaxScale) * modifiedWorldTransform.GetPosition().z * FixedPoint64(0.1f));
	}

	return std::vector<RenderingCommand> { RenderingCommand(_mesh, modifiedWorldTransform, _material) };
}
