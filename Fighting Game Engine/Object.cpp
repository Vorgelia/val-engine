#include "Object.h"
#include "Transform.h"
#include "Mesh.h"
#include "Material.h"
#include "IntroBehaviour.h"
#include "Resource.h"
#include "Rendering.h"
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
		if(iter.second != nullptr)
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

Object::Object(const json & j, ServiceManager* serviceManager)
{
	_serviceManager = serviceManager;

	enabled = j["enabled"].get<bool>();
	_name = j["name"].get<std::string>();
	_id = j["id"].get<int>();

	for(auto& iter : j["behaviours"])
	{
		std::string& behaviourName = iter["name"].get<std::string>();
		Behaviour* addedBehaviour = AddBehaviour(behaviourName, iter);
	}
}
