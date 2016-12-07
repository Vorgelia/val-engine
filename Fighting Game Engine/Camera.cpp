#include "Camera.h"


Camera::Camera(glm::vec2 pos, glm::mat4* mat){
	this->position = pos;
	this->projectionMatrix = mat;
}

glm::mat4 Camera::ViewMatrix(){
	glm::mat4 view;
	glm::translate(view,glm::vec3(-this->position,0));
	//view=glm::inverse(view);
	return view;
}