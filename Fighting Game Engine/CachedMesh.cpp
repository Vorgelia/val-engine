#include "CachedMesh.h"
#include "Mesh.h"
#include "ResourceLoader.h"
#include "DebugLog.h"

//Cached meshes are just raw vertex data that are stored in memory before being turned into meshes.
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

bool CachedMesh::RegisterOwner(Mesh* owner)
{
	auto& result = owners.insert(owner);
	if(result.second)
	{
		owner->SetMeshData(this);
	}
	return result.second;
}
bool CachedMesh::UnregisterOwner(Mesh* owner)
{
	auto& iter = owners.find(owner);
	if(iter != owners.end())
	{
		owner->SetMeshData(nullptr);
		owners.erase(iter);
		return true;
	}
	return false;
}
CachedMesh::CachedMesh(const std::string& name, std::vector<float>& verts, std::vector<GLuint>& elements, const std::vector<VertexAttribute>& vertexFormat)
{
	this->name = name;
	this->verts = verts;
	this->elements = elements;
	this->vertexFormat = vertexFormat;
}

CachedMesh::CachedMesh(const FS::path& path)
{
	this->name = path.string();
	if (path.extension().string() == ".vm")
		ResourceLoader::LoadMeshVM(path, verts, elements, vertexFormat);
	else
	{
		DebugLog::Push("Unhandled mesh format: " + path.extension().string());
		//TODO: Handle importing other meshes
	}
}

CachedMesh::~CachedMesh()
{
	for(auto& i : owners)
	{
		owners.erase(i);
		i->SetMeshData(nullptr);
	}
}
