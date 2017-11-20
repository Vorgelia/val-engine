#include "Transform.h"
#include "JSON.h"
#include <GLM\gtc\matrix_transform.hpp>
#include <GLM\gtc\type_ptr.hpp>
#include "BehaviourFactory.h"

VE_BEHAVIOUR_REGISTER_TYPE(Transform);

glm::mat4 Transform::ModelMatrix()
{
	glm::mat4 tl, rot, sc;
	tl = glm::translate(glm::mat4(), glm::vec3((float)position.x, (float)position.y, -1.0 + 1.0 / (1.0 + glm::abs(depth))));
	rot = glm::mat4_cast(rotation);
	sc = glm::scale(glm::mat4(), glm::vec3(this->scale.x, this->scale.y, 1));
	return tl*rot*sc;
}

void Transform::SnapTo(const Transform& tr)
{
	position = tr.position;
	rotation = tr.rotation;
	depth = tr.depth;
	scale = tr.scale;
}

Transform::Transform(Object* owner, glm::ivec2 position, glm::vec3 eulerRotation, glm::vec2 scale) : Behaviour(owner)
{
	this->position = position;
	this->scale = scale;
	this->rotation = glm::quat(rotation);
	this->depth = 0;
}

Transform::Transform(Object* owner, glm::ivec2 position, glm::quat rotation, glm::vec2 scale) : Behaviour(owner)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->depth = 0;
}

Transform::Transform(Object* owner, const json & j) : Behaviour(owner, j)
{
	position = JSON::Get<glm::ivec2>(j["position"]);
	rotation = JSON::Get<glm::quat>(j["rotation"]);
	scale = JSON::Get<glm::vec2>(j["scale"]);
	depth = j["depth"].get<float>();
}

Transform::Transform(Object* owner) : Behaviour(owner)
{
	this->position = glm::ivec2(0, 0);
	this->scale = glm::vec2(1, 1);
	this->rotation = glm::quat();
	this->depth = 0;
}