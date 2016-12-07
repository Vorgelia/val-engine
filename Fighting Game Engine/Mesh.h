#pragma once
#include "CommonUtilIncludes.hpp"
class CachedMesh;

class Mesh
{
private:
	bool _valid;
	
public:
	std::string name;
	CachedMesh *meshData;
	
	GLuint vao;
	GLuint vbo;
	GLuint ebo;

	GLuint elementAmount;

	bool editable;//If set to true, prevent the vertices from being deleted while this mesh exists
	bool valid();
	Mesh(std::string name, CachedMesh* meshData, bool editable);
	void Update();
	~Mesh();
};

