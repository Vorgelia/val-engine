#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"

VE_OBJECT_DEFINITION(GameObject);

ObjectReference<ObjectComponent> GameObject::AddComponentFromJson(const json& j)
{
	_components.push_back(ObjectFactory::CreateObjectFromJson<ObjectComponent>(this, j));
	ObjectComponent* result = (_components.back().get());

	return ObjectReference<ObjectComponent>(result);
}

json GameObject::Serialize() const
{
	json parentJson = SceneObject::Serialize();

	json componentJson = json::array();
	for(const auto& iter : _components)
	{
		componentJson.push_back(iter->Serialize());
	}

	parentJson.emplace(_owningInstance->configData().serializationConfigData.componentListPropertyName, componentJson);
	return parentJson;
}

void GameObject::Deserialize(const json& j)
{
	SceneObject::Deserialize(j);

	const EngineConfigData& configData = _owningInstance->configData();

	json objectJson;
	if(JSON::TryGetMember(j, configData.serializationConfigData.componentListPropertyName, objectJson) && objectJson.is_array())
	{
		for(const json& componentJson : objectJson)
		{
			AddComponentFromJson(componentJson);
		}
	}
}
