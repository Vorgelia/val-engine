#pragma once
#include "CommonUtilIncludes.hpp"
class Transform
{
	glm::ivec2 _position;
public:
	
	glm::vec2 scale;
	glm::quat rotation;
	float depth;

	glm::mat4 ModelMatrix();

	void position(glm::ivec2 pos);
	glm::ivec2 position();
	void Translate(glm::vec2 offset);
	Transform(glm::vec2 position, glm::vec3 rotation, glm::vec2 scale);
	Transform(glm::vec2 position, glm::quat rotation, glm::vec2 scale);
};

