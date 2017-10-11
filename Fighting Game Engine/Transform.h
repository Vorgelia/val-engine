#pragma once
#include <GLM\glm.hpp>
#include <GLM\gtc\quaternion.hpp>

class Transform
{
public:
	glm::ivec2 position;
	glm::vec2 scale;
	glm::quat rotation;
	float depth;

	glm::mat4 ModelMatrix();

	Transform(glm::ivec2 position, glm::vec3 rotation, glm::vec2 scale);
	Transform(glm::ivec2 position, glm::quat rotation, glm::vec2 scale);
	Transform();
};

