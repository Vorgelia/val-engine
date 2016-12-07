#pragma once
#include "CommonUtilIncludes.hpp"
class Camera
{
public:
	glm::vec2 position;
	glm::mat4* projectionMatrix;
	glm::mat4 ViewMatrix();
	Camera(glm::vec2 pos, glm::mat4* mat);
};

