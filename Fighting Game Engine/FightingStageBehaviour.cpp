#include "FightingStageBehaviour.h"

VE_BEHAVIOUR_REGISTER_TYPE(FightingStageBehaviour);

const glm::lvec4& FightingStageBehaviour::stageBounds() const
{
	return _stageBounds;
}

FightingStageBehaviour::FightingStageBehaviour(Object* owner, ServiceManager* serviceManager, const json& j)
	: Behaviour(owner, serviceManager, j)
{
	JSON::TryGetMember(j, "stageBounds", _stageBounds);
}
