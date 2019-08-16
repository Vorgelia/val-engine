#include "SerializationProxy.h"
#include "IReflectable.h"
#include "ReflectionField.h"

std::unique_ptr<BaseSerializationProxy> JsonSerializationProxy::GetSubProxy(const std::string& name) const
{
	json j;
	if(!JSON::TryGetMember(_json, name, j))
	{
		return nullptr;
	}
	return std::make_unique<JsonSerializationProxy>(j);
}

std::unique_ptr<BaseSerializationProxy> LuaSerializationProxy::GetSubProxy(const std::string& name) const
{
	std::string newName = _objectName + "." + name;
	if (!_state[newName.c_str()].exists())
	{
		return nullptr;
	}

	return std::make_unique<LuaSerializationProxy>(_state, std::move(newName));
}
