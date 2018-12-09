#include "ObjectFactory.h"
#include "GameInstance.h"

void ve::ObjectDeleter::operator()(BaseObject* object) const
{
	object->OnDestroyed();
	delete object;
}

ObjectFactory::BaseObjectGeneratorMap* ObjectFactory::objectGenerators()
{
	static std::unique_ptr<BaseObjectGeneratorMap> generatorMap = std::make_unique<BaseObjectGeneratorMap>();
	return generatorMap.get();
}

const EngineConfigData& ObjectFactory::GetEngineConfigData(BaseObject* contextObject)
{
	return contextObject->owningInstance()->configData();
}

void ObjectFactory::InitializeObject(BaseObject* object, BaseObject* outer, const json& j)
{
	ObjectInitializer objectInitializer{ outer };
	objectInitializer.InitializeObject(object);

	if(!j.empty())
	{
		object->Deserialize(j);
	}

	object->OnInit();
}
