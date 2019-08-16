#include "ObjectFactory.h"
#include "GameInstance.h"
#include "BaseObject.h"
#include "SerializationProxy.h"
#include "ResourceManager.h"

ObjectFactory::BaseObjectGeneratorMap* ObjectFactory::objectGenerators()
{
	static std::unique_ptr<BaseObjectGeneratorMap> generatorMap = std::make_unique<BaseObjectGeneratorMap>();
	return generatorMap.get();
}

const EngineConfigData& ObjectFactory::GetEngineConfigData(BaseObject* contextObject)
{
	return contextObject->owningInstance()->configData();
}

json* ObjectFactory::GetJsonResource(const std::string& name, BaseObject* contextObject)
{
	return contextObject->owningInstance()->ResourceManager()->GetJsonData(name);
}

void ObjectFactory::InitializeObject(BaseObject* object, BaseObject* outer, BaseSerializationProxy* proxy)
{
	ObjectInitializer objectInitializer{ outer };
	objectInitializer.InitializeObject(object);

	if(proxy != nullptr)
	{
		const SerializationConfigData& serializationData = GetEngineConfigData(outer).serializationConfigData;

		std::string prefabPath;
		if (proxy->Get<std::string>(serializationData.prefabPathPropertyName, prefabPath))
		{
			json* j = GetJsonResource(prefabPath, outer);
			if (j != nullptr)
			{
				JsonSerializationProxy jsonProxy(*j);
				static_cast<IReflectable*>(object)->DeserializeProxy(jsonProxy);
			}
		}

		static_cast<IReflectable*>(object)->DeserializeProxy(*proxy);
	}

	object->OnInit();
}
