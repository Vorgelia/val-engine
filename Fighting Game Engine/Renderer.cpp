#include "Renderer.h"
#include "RenderingGL.h"
#include "ResourceManager.h"
#include "GameInstance.h"
#include <string>
#include "ObjectFactory.h"
#include "Mesh.h"

VE_OBJECT_DEFINITION(BaseRenderer);
VE_OBJECT_DEFINITION(Renderer);

json Renderer::Serialize() const
{
	json outJson = BaseRenderer::Serialize();
	if(_material != nullptr)
	{
		outJson.emplace("material", _material->name);
	}
	if(_mesh != nullptr)
	{
		outJson.emplace("mesh", _mesh->name);
	}
	return outJson;
}

void Renderer::Deserialize(const json& j)
{
	std::string resourcePath;
	if(JSON::TryGetMember(j, "material", resourcePath))
	{
		_material = _owningInstance->ResourceManager()->GetMaterial(resourcePath);
	}
	if(JSON::TryGetMember(j, "mesh", resourcePath))
	{
		_mesh = _owningInstance->ResourceManager()->GetMesh(resourcePath);
	}
}

std::vector<RenderingCommand> Renderer::GetRenderingCommands() const
{
	return std::vector<RenderingCommand> { RenderingCommand(_mesh, GetWorldTransform(), _material) };
}
