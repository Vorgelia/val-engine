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
	glm::mat4 tl, rot, sc;
	tl = glm::translate(glm::mat4(), glm::vec3((float)position.x / (1.0 + depth), (float)position.y / (1.0 + depth), -1.0+1.0 / (1.0 + glm::abs(depth))));
	rot = glm::mat4_cast(rotation);
	sc = glm::scale(glm::mat4(), glm::vec3(this->scale.x, this->scale.y, 1));
	return tl*rot*sc;
}