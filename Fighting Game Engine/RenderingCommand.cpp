#include "RenderingCommand.h"

RenderingCommand::RenderingCommand(Mesh* inMesh, Transform inTransform, Material* inMaterial)
	: mesh(inMesh)
	, transform(std::move(inTransform))
	, material(inMaterial)
{
	
}
