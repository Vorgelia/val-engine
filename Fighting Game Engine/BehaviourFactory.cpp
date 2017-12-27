#include "BehaviourFactory.h"
#include "ServiceManager.h"
#include "Behaviour.h"
#include "Object.h"

BehaviourGeneratorMap* BehaviourFactory::_objectGenerators()
{
	static std::unique_ptr<BehaviourGeneratorMap> map = std::make_unique<BehaviourGeneratorMap>();
	return map.get();
}

std::unique_ptr<Behaviour> BehaviourFactory::Create(std::string name, Object* owner, ServiceManager* serviceManager, const json& j)
{
	auto& iter = _objectGenerators()->find(name);
	if(iter != _objectGenerators()->end())
	{
		return iter->second(owner, serviceManager, j);
	}

	return nullptr;
}
