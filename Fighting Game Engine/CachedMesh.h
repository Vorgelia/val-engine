#pragma once
#include <vector>
#include <string>
#include <unordered_set>
#include <filesystem>
#include "GLIncludes.hpp"

class Mesh;
enum class VertexAttributeLocation
{
	Vertex = 0,
	UV = 1,
	Normal = 2
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
	std::vector<float> verts;
	std::vector<GLuint> elements;
	std::vector<VertexAttribute> vertexFormat;

	std::unordered_set<Mesh*> owners;

	bool RegisterOwner(Mesh*);
	bool UnregisterOwner(Mesh*);

	CachedMesh(const std::string& name, std::vector<float>& verts, std::vector<GLuint>& elements, const std::vector<VertexAttribute>& vertexFormat);
	CachedMesh(std::string name);
	~CachedMesh();
};

