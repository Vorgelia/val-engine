#pragma once
#include "CommonUtilIncludes.hpp"
class Camera
{
public:
	glm::vec2 position;
	glm::mat4* projectionMatrix;
	float zoomLevel;
	glm::mat4 ViewMatrix(float depth = 1);
	Camera(glm::vec2 pos, glm::mat4* mat);
};

