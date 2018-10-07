#include "GameObject.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Renderer.h"

std::string GameObject::name() const
{
	return _name;
}

int GameObject::id() const
{
	return _id;
}

void GameObject::RunFunctionOnBehaviours(std::function<void(Behaviour*)> func)
{
	if(!enabled)
	{
		return;
	}

	for(auto& iter : _behaviours)
	{
		if(iter.second != nullptr && iter.second->enabled)
		{
			func(iter.second.get());
		}
	}
}

GameObject::GameObject()
{
	
}

GameObject::GameObject(const std::string& name, GameInstance* serviceManager, int id)
{
	this->_name = name;
	this->_id = id;
}

GameObject::GameObject(const json & j, GameInstance* serviceManager, int id)
{
	_serviceManager = serviceManager;
	_id = id;

	for(auto& iter : j["behaviours"])
	{
		const std::string behaviourName = iter["name"].get<std::string>();
		AddBehaviour(behaviourName, iter);
	}
}
