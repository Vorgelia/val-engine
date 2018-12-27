#include "CachedMesh.h"
#include "Mesh.h"
#include <utility>
#include "FilesystemManager.h"

//Cached meshes are just raw vertex data that is stored in memory before being turned into meshes.
//Helps with duplicating or editing meshes in realtime.
//Probably unnecessary.
VertexAttribute::VertexAttribute(VertexAttributeLocation ind, GLint len)
{
	index = ind;
	length = len;
}

std::vector<VertexAttribute> VertexAttribute::defaultMesh()
{
	return{ VertexAttribute(VertexAttributeLocation::Vertex, 2), VertexAttribute(VertexAttributeLocation::UV, 2), VertexAttribute(VertexAttributeLocation::Normal, 3) };
}

CachedMesh::CachedMesh(const std::string& name, std::vector<float>& verts, std::vector<GLuint>& elements, const std::vector<VertexAttribute>& vertexFormat)
{
	this->name = name;
	this->verts = verts;
	this->elements = elements;
	this->vertexFormat = vertexFormat;
}

CachedMesh::CachedMesh(std::string name) : name(std::move(name)) {}

CachedMesh::~CachedMesh()
= default;
