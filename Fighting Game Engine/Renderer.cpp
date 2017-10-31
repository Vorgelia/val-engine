#include "Renderer.h"
#include "Rendering.h"
#include <string>

void Renderer::OnSceneRenderPass()
{
	if(_owner->transform() == nullptr)
	{
		return;
	}
	Rendering::DrawMesh(_owner->transform(), _mesh, _material);
}

Renderer::Renderer(Object* owner, Mesh* mesh, Material* material) :Behaviour(owner)
{
	_mesh = mesh;
	_material = material;
}

Renderer::Renderer(Object* owner, const json& j) : Behaviour(owner, j)
{
	auto& iter = j.find("mesh");
	if(iter != j.end())
	{
		_mesh = Resource::GetMesh(iter->get<std::string>());
	}
	else
	{
		_mesh = nullptr;
	}

	iter = j.find("material");
	if(iter != j.end())
	{
		_material = Resource::GetMaterial(iter->get<std::string>());
	}
	else
	{
		_material = nullptr;
	}
}

Renderer::~Renderer()
{
}
