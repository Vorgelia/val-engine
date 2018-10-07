#include "ObjectFactory.h"

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
