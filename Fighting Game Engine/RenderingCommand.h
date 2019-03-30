#pragma once
#include "Transform.h"
#include "Material.h"
#include "ObjectReferenceManager.h"

class Mesh;
class BaseCamera;

class RenderingCommand
{
public:
	Mesh* mesh;
	Transform transform;
	Material* material;

	RenderingCommand(Mesh* inMesh, Transform inTransform, Material* inMaterial);
	~RenderingCommand() = default;
};

