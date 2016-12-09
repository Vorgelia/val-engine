#include "Hitbox.h"

Hitbox::Hitbox(glm::ivec4 rect){
	this->rect = rect;
}

bool Hitbox::Overlap(Hitbox* hitbox){
	return false;
}