#include "Camera.h"

Camera::Camera(glm::vec2 pos, glm::mat4* mat)
{
	this->position = pos;
	this->projectionMatrix = mat;
	zoomLevel = 1;
}

glm::mat4 Camera::ViewMatrix(float depth)
{
	glm::mat4 view;
	view = glm::translate(view, glm::vec3(-this->position / (1 + glm::abs(depth)), 0));
	view = glm::scale(view, glm::vec3(zoomLevel, zoomLevel, 1));
	return view;
}