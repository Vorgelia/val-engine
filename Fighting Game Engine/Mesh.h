#pragma once
#include <string>
#include "GLIncludes.hpp"

class CachedMesh;

class Mesh
{
	friend class GraphicsGL;

private:
	CachedMesh* _meshData;

	GLuint _vao;
	GLuint _vbo;
	GLuint _ebo;

	GLuint _elementAmount;

public:
	const std::string name;

	GLuint elementAmount() const;

	Mesh(std::string name, CachedMesh* meshData);
	~Mesh();
};