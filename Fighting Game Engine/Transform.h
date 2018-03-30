#pragma once
#include "MathIncludes.hpp"
#include "Behaviour.h"

class Transform : public Behaviour
{
public:
	VE_BEHAVIOUR_NAME(Transform);

	glm::lvec2 position;
	glm::vec2 scale;
	glm::quat rotation;
	float depth;

	glm::mat4 ModelMatrix() const;

	void SnapTo(const Transform& tr);

	Transform(Object* owner, ServiceManager* serviceManager, glm::lvec2 position, glm::vec3 eulerRotation, glm::vec2 scale);
	Transform(Object* owner, ServiceManager* serviceManager, glm::lvec2 position, glm::quat rotation, glm::vec2 scale);
	Transform(Object* owner, ServiceManager* serviceManager, const json& j);
	Transform(Object* owner, ServiceManager* serviceManager);
};

