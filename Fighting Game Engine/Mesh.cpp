#include "Mesh.h"
#include "CachedMesh.h"
#include "GLStateTrack.h"
#include "DebugLog.h"

Mesh::Mesh(const std::string& name, CachedMesh* meshData, bool editable = false)
{
	this->name = name;

	vbo = ebo = vao = 0;
	_valid = false;
	this->editable = editable;

	if(meshData != nullptr)
	{
		meshData->RegisterOwner(this);
	}
}

//Also something very easily omitted from a game that heavily relies on screen aligned quads.
//Updates the mesh, generating the necessary buffers if they haven't been generated already.
void Mesh::Update()
{
	if(meshData == nullptr)
	{
		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &vao);
		_valid = false;
		return;
	}

	if(meshData->verts.size() == 0)
		return;

	if(!_valid)
		glGenVertexArrays(1, &vao);

	GLState::BindVertexArray(vao);
	if(!_valid)
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);//Only bind buffer the first time the mesh is updated. They are automatically bound with the VAO later.
	}

	glBufferData(GL_ARRAY_BUFFER, meshData->verts.size()*sizeof(float), &(meshData->verts.at(0)), GL_STATIC_DRAW);
	if(!_valid)
	{
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData->elements.size()*sizeof(GLuint), &(meshData->elements.at(0)), GL_STATIC_DRAW);
	elementAmount = meshData->elements.size();

	//Vertex attrib automation
	unsigned int i = 0;
	int totalSize = 0;
	for(i = 0; i < meshData->vertexFormat.size(); ++i)
	{
		//Get total size of every vertex
		totalSize += meshData->vertexFormat[i].length;
	}

	int stride = 0;
	for(i = 0; i < meshData->vertexFormat.size(); ++i)
	{
		glEnableVertexAttribArray((GLuint)meshData->vertexFormat[i].index);
		glVertexAttribPointer((GLuint)meshData->vertexFormat[i].index, meshData->vertexFormat[i].length, GL_FLOAT, GL_FALSE, sizeof(float)*totalSize, (void*)(sizeof(float)*stride));
		stride += meshData->vertexFormat[i].length;
	}

	_valid = true;
}

void Mesh::SetMeshData(CachedMesh* meshData)
{
	this->meshData = meshData;
	Update();
}

bool Mesh::valid()
{
	return _valid;
}

Mesh::~Mesh()
{
	if(meshData != nullptr)
		meshData->UnregisterOwner(this);
}
