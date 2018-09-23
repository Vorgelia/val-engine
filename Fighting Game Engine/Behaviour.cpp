#include "Behaviour.h"

void Behaviour::TryInit()
{
	if(!_initialized)
	{
		VE_BEHAVIOUR_FUNCTION_CALLER(Init)(this);
		_initialized = true;
	}
}

void Behaviour::Init()
{

}

void Behaviour::OnSceneInit()
{

}

void Behaviour::EngineUpdate()
{

}

void Behaviour::GameUpdate()
{

}

void Behaviour::LateGameUpdate()
{

}

void Behaviour::LateEngineUpdate()
{

}

void Behaviour::OnRenderObjects()
{

}

void Behaviour::OnApplyPostEffects()
{

}

void Behaviour::OnRenderUI()
{

}

void Behaviour::Cleanup()
{

}

Behaviour::Behaviour(Object* owner, ServiceManager* serviceManager)
{
	assert(owner != nullptr);
	_owner = owner;
	enabled = false;
}

Behaviour::Behaviour(Object* owner, ServiceManager* serviceManager, const json& j)
{
	assert(owner != nullptr);
	_owner = owner;
	_initialized = false;
	_serviceManager = serviceManager;
	
	JSON::TryGetMember<bool>(j, "enabled", enabled);
}

Behaviour::~Behaviour()
{
	VE_BEHAVIOUR_FUNCTION_CALLER(Cleanup)(this);
}

Object* Behaviour::object() const
{
	return _owner;
}
