#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>
#include "Behaviour.h"

class Transform : public Behaviour
{
public:
	VE_BEHAVIOUR_NAME(Transform);

	glm::ivec2 position;
	glm::vec2 scale;
	glm::quat rotation;
	float depth;

	glm::mat4 ModelMatrix();

	void SnapTo(const Transform& tr);

	Transform(Object* owner, glm::ivec2 position, glm::vec3 eulerRotation, glm::vec2 scale);
	Transform(Object* owner, glm::ivec2 position, glm::quat rotation, glm::vec2 scale);
	Transform(Object* owner, const json& j);
	Transform(Object* owner);
};

