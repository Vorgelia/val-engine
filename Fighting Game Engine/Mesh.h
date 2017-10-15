#pragma once
#include <string>
#include "GLIncludes.hpp"

class CachedMesh;

class Mesh
{
private:
	bool _valid;

public:
	std::string name;
	CachedMesh* meshData;

	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLuint elementAmount;

	bool editable;//If set to true, prevent the vertices from being deleted while this mesh exists
	bool valid();
	
	void Update();

	void SetMeshData(CachedMesh* meshData);

	Mesh(const std::string& name, CachedMesh* meshData, bool editable);
	~Mesh();
};

