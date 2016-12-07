#include "Transform.h"


Transform::Transform(glm::ivec2 position, glm::vec3 rotation, glm::vec2 scale){
	this->position = position;
	this->scale = scale;
	this->rotation = glm::quat(rotation);
	this->depth = 0;
}
Transform::Transform(glm::ivec2 position, glm::quat rotation, glm::vec2 scale){
	this->position = position;
	this->scale = scale;
	this->rotation = rotation;
	this->depth = 0;
}
Transform::Transform(){
	this->position = glm::ivec2(0,0);
	this->scale = glm::vec2(1, 1);
	this->rotation = glm::quat();
	this->depth = 0;
}
glm::mat4 Transform::ModelMatrix(){
	glm::mat4 model;
	model=glm::translate(model, glm::vec3((float)position.x / (1.0 + depth), (float)position.y / (1.0 + depth), 1.0 - 1.0 / (1 + depth)));
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1));
	model *= glm::mat4_cast(rotation);
	return model;
}