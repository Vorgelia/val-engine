#include "ScriptArray.h"

void ScriptArray::EnsureIndexInRange(int index) const
{
	if(index < 0 || index >= int(_storage.size()))
	{
		throw ScriptError("Attempting to access array element with index out of range.");
	}
}

std::shared_ptr<ScriptArray::value_type> ScriptArray::GetMember(const std::shared_ptr<key_type>& key)
{
	const int intValue(key->value());
	EnsureIndexInRange(intValue);
	return _storage[intValue];
}

void ScriptArray::RemoveMember(const std::shared_ptr<key_type>& key)
{
	const int intValue(key->value());
	EnsureIndexInRange(intValue);
	_storage.erase(_storage.begin() + intValue);
}

std::shared_ptr<ScriptArray::value_type> ScriptArray::Back()
{
	if(_storage.empty())
	{
		return nullptr;
	}
	return _storage.back();
}

void ScriptArray::Clear()
{
	_storage.clear();
}

std::shared_ptr<ScriptDec> ScriptArray::Size() const
{
	return std::make_shared<ScriptDec>(FixedPoint64(int(_storage.size())));
}

std::shared_ptr<ScriptArray::value_type> ScriptArray::Push(const std::shared_ptr<value_type>& value)
{
	_storage.push_back(value->Clone());
	return _storage.back();
}

std::shared_ptr<ScriptArray::value_type> ScriptArray::Pop()
{
	std::shared_ptr<value_type> poppedElement = std::make_shared<value_type>(*_storage.back());
	_storage.pop_back();
	return poppedElement;
}

json ScriptArray::ToJSON() const
{
	json parentJson = Super::ToJSON();
	/*
	json& arrayJson = (parentJson["collection"] = json{});
	for(auto& iter : _storage)
	{
		arrayJson.emplace(iter->ToJSON());
	}
	*/
	return parentJson;
}

std::shared_ptr<BaseScriptVariable> ScriptArray::Clone() const
{
	return std::make_shared<ScriptArray>(*this);
}

ScriptArray::ScriptArray(bool isConst) 
	: Super(isConst)
{
	_initialized = true;
}

ScriptArray::ScriptArray(const json& j)
{
	json keyList;
	if(!JSON::TryGetMember(j, "collection", keyList))
	{
		return;
	}

	for(const json& iter : keyList)
	{
		_storage.push_back(std::make_shared<value_type>(iter));
	}
}

ScriptArray::ScriptArray(const ScriptArray& scriptArray) 
	: Super(scriptArray.isConst())
{
	for(auto& iter : scriptArray._storage)
	{
		_storage.push_back(iter->Clone());
	}

	_initialized = true;
	_const = false;
}
