#include "SerializationProxy.h"
#pragma once

#define DEFERRED_PROXY_GET(ProxyType, OpType, VarName)\
	if(auto proxy = dynamic_cast<const ProxyType##SerializationProxy*>(this))\
		return proxy->Get##OpType<VarType>(name, VarName)
#define DEFERRED_PROXY_SET(ProxyType, OpType, VarName)\
	if(auto proxy = dynamic_cast<ProxyType##SerializationProxy*>(this))\
	{\
		proxy->Set##OpType<VarType>(name, VarName);\
		return;\
	}\


template <typename VarType>
bool BaseSerializationProxy::Get(const std::string& name, VarType& out_var) const
{
	DEFERRED_PROXY_GET(Json, , out_var);
	DEFERRED_PROXY_GET(Lua, , out_var);
	return false;
}

template <typename VarType>
void BaseSerializationProxy::Set(const std::string& name, const VarType& var)
{
	DEFERRED_PROXY_SET(Json, , var);
	DEFERRED_PROXY_SET(Lua, , var);
}

template <typename VarType>
void BaseSerializationProxy::GetObject(const std::string& name, ve::unique_object_ptr<VarType>& out_var, BaseObject* outer) const
{
	if (auto proxy = dynamic_cast<const JsonSerializationProxy*>(this))
	{
		proxy->GetObject<VarType>(name, out_var, outer);
		return;
	}
	if (auto proxy = dynamic_cast<const LuaSerializationProxy*>(this))
	{
		proxy->GetObject<VarType>(name, out_var, outer);
		return;
	}
}

template <typename VarType>
bool BaseSerializationProxy::GetArray(const std::string& name, std::vector<VarType>& out_vec) const
{
	DEFERRED_PROXY_GET(Json, Array, out_vec);
	DEFERRED_PROXY_GET(Lua, Array, out_vec);
	return false;
}

template <typename VarType>
void BaseSerializationProxy::SetArray(const std::string& name, const std::vector<VarType>& vec)
{
	DEFERRED_PROXY_SET(Json, Array, vec);
	DEFERRED_PROXY_SET(Lua, Array, vec);
}

template<typename VarType>
inline void BaseSerializationProxy::FillObjectArray(const std::string& name, std::vector<VarType>& vec, BaseObject* outer) const
{
	if (auto proxy = dynamic_cast<const JsonSerializationProxy*>(this))
	{
		proxy->FillObjectArray<VarType>(name, vec, outer);
		return;
	}
	if (auto proxy = dynamic_cast<const LuaSerializationProxy*>(this))
	{
		proxy->FillObjectArray<VarType>(name, vec, outer);
		return;
	}
}

template <typename KeyType, typename ValueType>
bool BaseSerializationProxy::GetMap(const std::string& name, std::unordered_map<KeyType, ValueType>& out_map) const
{
	if (auto proxy = dynamic_cast<const JsonSerializationProxy*>(this))
		return proxy->GetMap<KeyType, ValueType>(name, out_map);
	if (auto proxy = dynamic_cast<const LuaSerializationProxy*>(this))
		return proxy->GetMap<KeyType, ValueType>(name, out_map);
}

template <typename KeyType, typename ValueType>
void BaseSerializationProxy::SetMap(const std::string& name, const std::unordered_map<KeyType, ValueType>& vec)
{
	if (auto proxy = dynamic_cast<JsonSerializationProxy*>(this))
	{
		proxy->SetMap<KeyType, ValueType>(name, vec);
		return;
	}
	if (auto proxy = dynamic_cast<LuaSerializationProxy*>(this))
	{
		proxy->SetMap<KeyType, ValueType>(name, vec);
		return;
	}
}