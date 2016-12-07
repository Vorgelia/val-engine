#include "CachedMesh.h"
#include "Mesh.h"
#include "ResourceLoader.h"

VertexAttribute::VertexAttribute(VertexAttributeLocation ind, GLint len){
	index = ind;
	length = len;
}

std::vector<VertexAttribute> VertexAttribute::defaultMesh(){
	return{ VertexAttribute(VertexAttributeLocation::vertex, 2), VertexAttribute(VertexAttributeLocation::uv, 2), VertexAttribute(VertexAttributeLocation::normal, 3) };
}

bool CachedMesh::RegisterOwner(Mesh* owner){
	for (auto iter = owners.begin(); iter < owners.end(); ++iter){
		if (*iter == owner){
			return false;
		}
	}
	owners.push_back(owner);
	owner->meshData = this;
	return true;
}
bool CachedMesh::UnregisterOwner(Mesh* owner){
	for (auto iter=owners.begin(); iter < owners.end(); ++iter){
		if (*iter != nullptr&&*iter == owner){
			(*iter)->meshData = nullptr;
			owners.erase(iter);
			return true;
		}
	}
	return false;
}
CachedMesh::CachedMesh(std::string name, std::vector<float> *verts, std::vector<GLuint> *elements, std::vector<VertexAttribute> vertexFormat){
	this->name = name;
	this->verts = verts;
	this->elements = elements;
	this->vertexFormat = vertexFormat;
}

CachedMesh::CachedMesh(FS::path path){
	this->name = path.string();
	//if (path.extension().string() == "vm")
		ResourceLoader::LoadMeshVM(path, &verts, &elements, &vertexFormat);
}


CachedMesh::~CachedMesh(){
	delete verts;
	delete elements;
	for (auto iter = owners.begin(); iter < owners.end(); ++iter){
		(*iter)->meshData = nullptr;
	}
}
