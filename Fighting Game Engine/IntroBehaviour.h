#pragma once
#include "Behaviour.h"

class IntroBehaviour :
	public Behaviour
{
	float _introDuration = 4;
public:
	VE_BEHAVIOUR_NAME(IntroBehaviour);

	VE_BEHAVIOUR_REGISTER_FUNCTION(OnSceneInit);
	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderUI);
	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);

	IntroBehaviour(Object* owner);
	IntroBehaviour(Object* owner, const json& j);
	~IntroBehaviour() = default;
};

