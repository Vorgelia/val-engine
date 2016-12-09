#include "Hitbox.h"

bool Hitbox::Overlap(Hitbox* hitbox){
	return (abs(rect.x - hitbox->rect.x) < rect.z + hitbox->rect.z) && (abs(rect.y - hitbox->rect.y) < rect.w + hitbox->rect.w);
}

Hitbox Hitbox::flipped(){
	return Hitbox(glm::vec4(-rect.x, rect.y, rect.z, rect.w));
}

Hitbox Hitbox::FromTopLeft(glm::vec4 rect){
	return Hitbox(glm::vec4(rect.x + rect.z*0.5, rect.y + rect.w*0.5, rect.z*0.5, rect.w*0.5));
}

Hitbox Hitbox::operator+(const glm::vec2 rhs){
	return Hitbox(glm::vec4(rect.x+rhs.x,rect.y+rhs.y,rect.z,rect.w));
}

Hitbox::Hitbox(glm::vec4 rect){
	this->rect = rect;
}
