#include "Mesh.h"
#include "CachedMesh.h"
#include "GraphicsGL.h"
#include "DebugLog.h"

Mesh::Mesh(const std::string& name, CachedMesh* meshData) :	name(name)
{
	_vbo = _ebo = _vao = 0;
	_meshData = meshData;
}

GLuint Mesh::elementAmount()
{
	return _elementAmount;
}

Mesh::~Mesh()
{
	if(_meshData != nullptr)
		_meshData->UnregisterOwner(this);
}