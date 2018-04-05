#include "Object.h"
#include "Transform.h"
#include "ResourceManager.h"
#include "Behaviour.h"
#include "Renderer.h"

std::string Object::name() const
{
	return _name;
}

int Object::id() const
{
	return _id;
}

Transform* Object::transform() const
{
	return _transform;
}

Renderer* Object::renderer() const
{
	return _renderer;
}

void Object::RunFunctionOnBehaviours(std::function<void(Behaviour*)> func)
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

Object::Object(const std::string& name, ServiceManager* serviceManager, int id)
{
	this->_name = name;
	this->_id = id;
}

Object::Object(const json & j, ServiceManager* serviceManager, int id)
{
	_serviceManager = serviceManager;
	_id = id;

	JSON::TryGetMember<bool>(j, "enabled", enabled);
	JSON::TryGetMember(j, "name", _name);

	for(auto& iter : j["behaviours"])
	{
		const std::string behaviourName = iter["name"].get<std::string>();
		AddBehaviour(behaviourName, iter);
	}
}
