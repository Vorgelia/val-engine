#pragma once
#include "CommonUtilIncludes.hpp"
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
	Object(const std::string& name, glm::vec2 pos, glm::vec2 scale, Mesh* mesh = Resource::meshes["Materials/Base/quad.vm"], Material* material = Resource::materials["Materials/Base/Object2D.vmat"], int id = 0);
	Object();
	~Object();
};