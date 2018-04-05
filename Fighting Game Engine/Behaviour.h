#pragma once
#include <string>
#include "Object.h"
#include "JSON.h"

#define VE_BEHAVIOUR_NAME(nameStr)\
	const std::string name() const override{ return std::string(#nameStr); };

#define VE_BEHAVIOUR_FUNCTION(name)\
	virtual void name(); \
	inline virtual bool using##name() const{ return false; };

#define VE_BEHAVIOUR_REGISTER_FUNCTION(name)\
	void name() override;\
	inline virtual bool using##name() const override{ return true; };

#define VE_BEHAVIOUR_FUNCTION_CALLER(name)\
	[](Behaviour* behaviour)\
		{ if(behaviour->using##name()) behaviour->name(); }

class ServiceManager;

class Behaviour
{
protected:
	Object* _owner;
	ServiceManager* _serviceManager;

private:
	bool _initialized;

public:
	bool enabled;

	Object* object() const;

	virtual const std::string name() const = 0;

	VE_BEHAVIOUR_FUNCTION(Init);
	VE_BEHAVIOUR_FUNCTION(OnSceneInit);
	VE_BEHAVIOUR_FUNCTION(EngineUpdate);
	VE_BEHAVIOUR_FUNCTION(GameUpdate);
	VE_BEHAVIOUR_FUNCTION(LateGameUpdate);
	VE_BEHAVIOUR_FUNCTION(LateUpdate);
	VE_BEHAVIOUR_FUNCTION(OnRenderObjects);
	VE_BEHAVIOUR_FUNCTION(OnApplyPostEffects);
	VE_BEHAVIOUR_FUNCTION(OnRenderUI);
	VE_BEHAVIOUR_FUNCTION(Cleanup);

	Behaviour(Object* owner, ServiceManager* serviceManager);
	Behaviour(Object* owner, ServiceManager* serviceManager, const json& j);
	virtual ~Behaviour();
};