#pragma once
#include <string>
#include <GLM\glm.hpp>
#include "Resource.h"

class Transform;
class Mesh;
class Material;

class Object
{
public:
	std::string name;
	unsigned int id;
	Transform* transform;
	Mesh* mesh;
	Material* material;
	bool render;

	void Render();

	Object(const std::string& name, glm::vec2 pos, glm::vec2 scale, Mesh* mesh = Resource::GetMesh("Materials/Base/quad.vm"), Material* material = Resource::GetMaterial("Materials/Base/Object2D.vmat"), int id = 0);
	Object();
	~Object();
};