#include "ObjectInitializer.h"
#include "BaseObject.h"
#include "SceneObject.h"
#include "GameObject.h"
#include "GameScene.h"
#include "ObjectComponent.h"
#include "GameInstance.h"

void ObjectInitializer::InitializeObject(BaseObject* other) const
{
	other->_owningInstance = _outerInstance;

	ObjectComponent* otherAsObjectComponent = dynamic_cast<ObjectComponent*>(other);
	SceneObject* otherAsSceneObject;
	if(otherAsObjectComponent != nullptr)
	{
		otherAsObjectComponent->_owner = _outerAsGameObject;
		otherAsObjectComponent->_transformParent = _outerAsGameObject;
		otherAsSceneObject = otherAsObjectComponent;
	}
	else
	{
		otherAsSceneObject = dynamic_cast<SceneObject*>(other);
	}

	if(otherAsSceneObject != nullptr)
	{
		otherAsSceneObject->_owningScene = _outerScene;
	}
}

ObjectInitializer::ObjectInitializer(BaseObject* outer)
	: _outer(outer)
	, _outerAsGameObject(nullptr)
	, _outerInstance(nullptr)
	, _outerScene(nullptr)
{
	if(outer == nullptr)
	{
		return;
	}

	GameInstance* outerAsGameInstance = dynamic_cast<GameInstance*>(outer);
	if(outerAsGameInstance != nullptr)
	{
		_outerInstance = outerAsGameInstance;
	}
	else
	{
		_outerInstance = outer->owningInstance();
	}

	SceneObject* outerAsSceneObject = dynamic_cast<SceneObject*>(outer);
	if(outerAsSceneObject != nullptr)
	{
		_outerScene = outerAsSceneObject->owningScene();
		_outerAsGameObject = dynamic_cast<GameObject*>(outer);
	}
	else
	{
		_outerScene = dynamic_cast<GameScene*>(outer);
	}
}