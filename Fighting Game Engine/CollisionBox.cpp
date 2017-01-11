#include "CollisionBox.h"

bool CollisionBox::Overlap(CollisionBox* hitbox){
	return (abs(rect.x - hitbox->rect.x) < rect.z + hitbox->rect.z) && (abs(rect.y - hitbox->rect.y) < rect.w + hitbox->rect.w);
}

CollisionBox CollisionBox::flipped(){
	return CollisionBox(glm::vec4(-rect.x, rect.y, rect.z, rect.w));
}

CollisionBox CollisionBox::FromTopLeft(glm::vec4 rect){
	return CollisionBox(glm::vec4(rect.x + rect.z*0.5, rect.y + rect.w*0.5, rect.z*0.5, rect.w*0.5));
}

CollisionBox CollisionBox::operator+(const glm::vec2 rhs){
	return CollisionBox(glm::vec4(rect.x + rhs.x, rect.y + rhs.y, rect.z, rect.w));
}

CollisionBox::CollisionBox(glm::vec4 rect){
	this->rect = rect;
}