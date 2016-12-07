#include "Object.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Resource.h"
#include "Rendering.h"
Object::Object(std::string name, glm::vec2 pos,glm::vec2 scale, Mesh* mesh, Material* material, int id){
	this->name = name;
	this->mesh = mesh;
	this->material = material;
	this->transform = new Transform(pos,glm::quat(),scale);
	this->id = id;
	this->render = true;
}

void Object::Render(){
	if (render)
		Rendering::DrawMesh(transform,mesh,material);
}

Object::~Object()
{
	delete transform;
}
