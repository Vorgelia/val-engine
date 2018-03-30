#pragma once
#include "Behaviour.h"
#include "MathIncludes.hpp"

class FightingStageBehaviour :
	public Behaviour
{
protected:
	glm::lvec4 _stageBounds;

	//TODO: Make characters send scene events.
	//void HandleSceneEvent();

public:
	VE_BEHAVIOUR_NAME(FightingStageBehaviour);

	const glm::lvec4& stageBounds() const;

	FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j);
	~FightingStageBehaviour() = default;
};

