#include "Object.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "Resource.h"
#include "Rendering.h"

std::string Object::name()
{
	return _name;
}

int Object::id()
{
	return _id;
}

Transform* Object::transform()
{
	return _transform.get();
}

void Object::Render()
{
	if(render)
		Rendering::DrawMesh(_transform.get(), _mesh, _material);
}

Object::Object(const std::string& name, glm::vec2 pos, glm::vec2 scale, Mesh* mesh, Material* material, int id)
{
	this->_name = name;
	this->_mesh = mesh;
	this->_material = material;
	this->_transform = std::make_unique<Transform>(pos, glm::quat(), scale);
	this->_id = id;
	this->render = true;
}

Object::Object(json j)
{
	_name = j["name"].get<std::string>();

	_mesh = Resource::GetMesh(
		j["mesh"].get<std::string>());

	_material = Resource::GetMaterial(
		j["material"].get<std::string>());

	_transform = std::make_unique<Transform>();

	_transform->position = glm::ivec2(
		j["position"]["x"].get<int>(),
		j["position"]["y"].get<int>());

	_transform->scale = glm::ivec2(
		j["scale"]["x"].get<int>(),
		j["scale"]["y"].get<int>());

	_transform->depth = j["depth"].get<float>();
	render = j["render"].get<bool>();
}

Object::Object()
{
	this->_name = "UNINITIALIZED";
	this->_mesh = nullptr;
	this->_material = nullptr;
	this->_transform = std::make_unique<Transform>();
	this->_id = 0;
	this->render = true;
}

Object::~Object()
{
}
