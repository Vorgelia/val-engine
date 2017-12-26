#include "Renderer.h"
#include "Rendering.h"
#include "Resource.h"
#include "ServiceManager.h"
#include <string>
#include "BehaviourFactory.h"

VE_BEHAVIOUR_REGISTER_TYPE(Renderer);

void Renderer::OnRenderObjects()
{
	if(_owner->transform() == nullptr)
	{
		return;
	}
	_rendering->DrawMesh(_owner->transform(), _mesh, _material);
}

Renderer::Renderer(Object* owner, ServiceManager* serviceManager, Mesh* mesh, Material* material) :Behaviour(owner, serviceManager)
{
	_mesh = mesh;
	if(_mesh == nullptr)
	{
		_mesh = _resource->GetMesh("Materials/Base/quad.vm");
	}

	_material = material;
	if(_material == nullptr)
	{
		_material = _resource->GetMaterial("Materials/Base/Object2D.vmat");
	}
}

Renderer::Renderer(Object* owner, ServiceManager* serviceManager, const json& j) : Behaviour(owner, serviceManager, j)
{
	_resource = serviceManager->ResourceManager();
	_rendering = serviceManager->Rendering();

	auto& iter = j.find("mesh");
	if(iter != j.end())
	{
		_mesh = _resource->GetMesh(iter->get<std::string>());
	}
	else
	{
		_mesh = nullptr;
	}

	iter = j.find("material");
	if(iter != j.end())
	{
		_material = _resource->GetMaterial(iter->get<std::string>());
	}
	else
	{
		_material = nullptr;
	}
}

Renderer::~Renderer()
{
}
