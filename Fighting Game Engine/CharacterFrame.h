#pragma once
#include "CommonUtilIncludes.hpp"
enum class ForceMode{
	Add=0,
	Set=1,
	//Combine=2 Unused for now. A force of f2 with magnitude m2 combined with a vector f1 of magnitude m1 would produce lerp(f1,f2,abs(m2-m1)/(m2+m1))
};
class CharacterFrame
{
public:
	glm::ivec2 movementOffset;
	glm::ivec2 force;
	ForceMode forceMode;

	//Hitboxes
	//Hurtboxes

	CharacterFrame();
	~CharacterFrame();
};

