#include "Transform.h"


Transform::Transform(glm::vec2 position, glm::vec3 rotation, glm::vec2 scale){
	this->_position = position;
	this->scale = scale;
	this->rotation = glm::quat(rotation);
}
Transform::Transform(glm::vec2 position, glm::quat rotation, glm::vec2 scale){
	this->_position = position;
	this->scale = scale;
	this->rotation = rotation;
}

glm::mat4 Transform::ModelMatrix(){
	glm::mat4 model;
	glm::translate(model, glm::vec3((float)_position.x / (1.0 + depth), (float)_position.y / (1.0 + depth), 1.0 - 1.0 / (1 + depth)));
	model*=glm::mat4_cast(rotation);
	glm::scale(model, glm::vec3(scale,1));
	
	return model;
}

void Transform::position(glm::ivec2 pos){
	_position = pos;
}
glm::ivec2 Transform::position(){
	return _position;
}
void Transform::Translate(glm::vec2 offset){
	_position += offset;
}