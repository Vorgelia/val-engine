#include "Mesh.h"
#include <utility>
#include "CachedMesh.h"
#include "DebugLog.h"

Mesh::Mesh(std::string name, CachedMesh* meshData) 
	: name(std::move(name))
{
	_vbo = _ebo = _vao = 0;
	_meshData = meshData;
}

GLuint Mesh::elementAmount() const
{
	return _elementAmount;
}