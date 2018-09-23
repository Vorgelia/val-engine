#pragma once
#include "MathIncludes.hpp"
#include "Behaviour.h"

class Transform : public Behaviour
{
public:
	VE_BEHAVIOUR_NAME(Transform);

	ve::vec2 position;
	ve::vec2 scale;
	glm::quat rotation;
	float depth;

	glm::mat4 ModelMatrix() const;

	void SnapTo(const Transform& tr);

	Transform(Object* owner, ServiceManager* serviceManager, ve::vec2 position, glm::vec3 eulerRotation, ve::vec2 scale);
	Transform(Object* owner, ServiceManager* serviceManager, ve::vec2 position, glm::quat rotation, ve::vec2 scale);
	Transform(Object* owner, ServiceManager* serviceManager, const json& j);
	Transform(Object* owner, ServiceManager* serviceManager);
};

