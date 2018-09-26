#pragma once
#include "Behaviour.h"
#include <string>

class ResourceManager;
class RenderingGL;

class Renderer :
	public Behaviour
{
protected:
	ResourceManager* _resource;
	RenderingGL* _rendering;

protected:
	Mesh* _mesh;
	Material* _material;

public:
	VE_BEHAVIOUR_NAME(Renderer);

	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderObjects);

	Renderer(Object* owner, GameInstance* serviceManager, Mesh* mesh = nullptr, Material* material = nullptr);
	Renderer(Object* owner, GameInstance* serviceManager, const json& j);
	~Renderer() = default;
};

