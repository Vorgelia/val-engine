#pragma once
#include "CommonUtilIncludes.hpp"

class Mesh;
enum class VertexAttributeLocation
{
	vertex = 0,
	uv = 1,
	normal = 2
};
//Leftover from my own work that didn't rely as much on screen aligned quads
//Automates management of vertex attrib pointers upon mesh creation
class VertexAttribute
{
public:
	VertexAttributeLocation index;
	GLint length;

	VertexAttribute(VertexAttributeLocation ind, GLint len);
	static std::vector<VertexAttribute> defaultMesh();
};
class CachedMesh
{
public:
	std::string name;
	std::vector<float> *verts;
	std::vector<GLuint> *elements;
	std::vector<VertexAttribute> vertexFormat;
	std::vector<Mesh*> owners;
	bool RegisterOwner(Mesh*);
	bool UnregisterOwner(Mesh*);
	CachedMesh(std::string name, std::vector<float> *verts, std::vector<GLuint> *elements, std::vector<VertexAttribute> vertexFormat);
	CachedMesh(FS::path path);
	~CachedMesh();
};

