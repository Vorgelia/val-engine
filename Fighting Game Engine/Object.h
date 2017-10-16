#pragma once
#include <string>
#include <GLM\glm.hpp>
#include <json.hpp>
#include "Resource.h"

class Transform;
class Mesh;
class Material;

using json = nlohmann::json;

class Object
{
	std::string _name;
	unsigned int _id;
	std::unique_ptr<Transform> _transform;
	Mesh* _mesh;
	Material* _material;
public:
	
	bool render;

	std::string name();
	int id();
	Transform* transform();

	void Render();

	Object(const std::string& name, glm::vec2 pos, glm::vec2 scale, Mesh* mesh = Resource::GetMesh("Materials/Base/quad.vm"), Material* material = Resource::GetMaterial("Materials/Base/Object2D.vmat"), int id = 0);
	Object(json j);
	Object();
	~Object();
};