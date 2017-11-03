#pragma once
#include "Behaviour.h"

class MenuBehaviour :
	public Behaviour
{
public:
	VE_BEHAVIOUR_NAME(MenuBehaviour);

	VE_BEHAVIOUR_REGISTER_FUNCTION(Update);
	VE_BEHAVIOUR_REGISTER_FUNCTION(GameUpdate);
	VE_BEHAVIOUR_REGISTER_FUNCTION(OnRenderUI);

	MenuBehaviour(Object* owner, const json& j);
	~MenuBehaviour();
};

