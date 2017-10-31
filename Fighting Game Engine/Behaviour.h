#pragma once
#include <string>
#include "Object.h"
#include "JSON.h"

#define VE_BEHAVIOUR_NAME(nameStr)\
	const std::string name() const override{ return std::string(#nameStr); };
#define VE_BEHAVIOUR_FUNCTION(name)\
	virtual void name(); inline virtual const bool using##name(){ return false; };
#define VE_BEHAVIOUR_REGISTER_FUNCTION(name)\
	void name() override; inline virtual const bool using##name() override{ return true; };
#define VE_BEHAVIOUR_FUNCTION_CALLER(name)\
	[](Behaviour* behaviour)\
		{ if(behaviour->using##name()) behaviour->name(); }

class Behaviour
{
protected:

	Object* _owner;

public:

	bool enabled;

	virtual const std::string name() const = 0;

	VE_BEHAVIOUR_FUNCTION(Init);
	VE_BEHAVIOUR_FUNCTION(Update);
	VE_BEHAVIOUR_FUNCTION(GameUpdate);
	VE_BEHAVIOUR_FUNCTION(OnSceneRenderPass);
	VE_BEHAVIOUR_FUNCTION(OnUIRenderPass);
	VE_BEHAVIOUR_FUNCTION(Cleanup);

	Behaviour(Object* owner);
	Behaviour(Object* owner, const json& j);
	~Behaviour();
};