#pragma once
#include "Behaviour.h"
#include "Resource.h"
#include <string>

class Renderer :
	public Behaviour
{
	Mesh* _mesh;
	Material* _material;
public:

	VE_BEHAVIOUR_NAME(Renderer);

	VE_BEHAVIOUR_REGISTER_FUNCTION(OnSceneRenderPass);

	Renderer(Object* owner, Mesh* mesh = Resource::GetMesh("Materials/Base/quad.vm"), Material* material = Resource::GetMaterial("Materials/Base/Object2D.vmat"));
	Renderer(Object* owner, const json& j);
	~Renderer();
};

