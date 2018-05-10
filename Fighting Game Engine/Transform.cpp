#include "Transform.h"
#include "JSON.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include "BehaviourFactory.h"

VE_BEHAVIOUR_REGISTER_TYPE(Transform);

glm::mat4 Transform::ModelMatrix() const
{
	const glm::mat4 translationMat = glm::translate(glm::mat4(), glm::vec3(float(position.x), float(position.y), -1.0 + 1.0 / (1.0 + glm::abs(depth))));
	const glm::mat4 rotationMat = glm::mat4_cast(rotation);
	const glm::mat4 scaleMat = glm::scale(glm::mat4(), glm::vec3(this->scale.x, this->scale.y, 1));
	return translationMat * rotationMat * scaleMat;
}

void Transform::SnapTo(const Transform& tr)
{
	position = tr.position;
	rotation = tr.rotation;
	depth = tr.depth;
	scale = tr.scale;
}

Transform::Transform(Object* owner, ServiceManager* serviceManager, ve::vec2 position, glm::vec3 eulerRotation, ve::vec2 scale) : Behaviour(owner, serviceManager)
{
	this->position = position;
	this->scale = scale;
	this->rotation = glm::quat(eulerRotation);
	this->depth = 0;
}

Transform::Transform(Object* owner, ServiceManager* serviceManager, ve::vec2 position, glm::quat rotation, ve::vec2 scale) : Behaviour(owner, serviceManager)
{
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->depth = 0;
}

Transform::Transform(Object* owner, ServiceManager* serviceManager, const json & j) : Behaviour(owner, serviceManager, j)
{
	position = JSON::Get<ve::vec2>(j["position"]);
	rotation = JSON::Get<glm::quat>(j["rotation"]);
	scale = JSON::Get<ve::vec2>(j["scale"]);
	depth = j["depth"].get<float>();
}

Transform::Transform(Object* owner, ServiceManager* serviceManager) : Behaviour(owner, serviceManager)
{
	this->position = ve::vec2(0, 0);
	this->scale = glm::vec2(1, 1);
	this->rotation = glm::quat();
	this->depth = 0;
}