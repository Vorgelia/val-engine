#include "Behaviour.h"

void Behaviour::Init()
{

}

void Behaviour::OnSceneInit()
{

}

void Behaviour::Update()
{

}

void Behaviour::GameUpdate()
{

}

void Behaviour::LateGameUpdate()
{

}

void Behaviour::LateUpdate()
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

Behaviour::Behaviour(Object* owner)
{
	assert(owner != nullptr);
	_owner = owner;
	enabled = false;
}

Behaviour::Behaviour(Object* owner, const json& j)
{
	assert(owner != nullptr);
	_owner = owner;
	enabled = j["enabled"].get<bool>();

	VE_BEHAVIOUR_FUNCTION_CALLER(Init)(this);
}

Behaviour::~Behaviour()
{
	VE_BEHAVIOUR_FUNCTION_CALLER(Cleanup)(this);
}