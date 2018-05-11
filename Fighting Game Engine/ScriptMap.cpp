#include "ScriptMap.h"
#include "ScriptError.h"


ScriptMap::value_type ScriptMap::AddMember(const key_type& key, const value_type& value)
{
	return _storage.emplace(key->value(), value->Clone()).first->second;
}

ScriptMap::value_type ScriptMap::GetMember(const key_type& key)
{
	auto& iter = _storage.find(key->value());
	if(iter == _storage.end())
	{
		throw ScriptError("Attempting to index invalid member of collection [" + key->value() + "]");
	}

	return iter->second;
}

void ScriptMap::RemoveMember(const key_type& key)
{
	_storage.erase(key->value());
}

void ScriptMap::Clear()
{
	_storage.clear();
}

std::shared_ptr<BaseScriptVariable> ScriptMap::Clone() const
{
	return std::make_shared<ScriptMap>(*this);
}

ScriptMap::ScriptMap(bool isConst) : BaseScriptCollection<ScriptString, BaseScriptVariable>(ScriptVariableType::Map, isConst)
{

}

ScriptMap::ScriptMap(const ScriptMap& map) : BaseScriptCollection<ScriptString, BaseScriptVariable>(ScriptVariableType::Map, map.isConst())
{
	for(auto& iter : map._storage)
	{
		_storage.emplace(iter.first, iter.second->Clone());
	}
}
