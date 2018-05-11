#include "ScriptArray.h"

void ScriptArray::EnsureIndexInRange(int index) const
{
	if(index < 0 || index >= int(_storage.size()))
	{
		throw ScriptError("Attempting to access array element with index out of range.");
	}
}

ScriptArray::value_type ScriptArray::GetMember(const key_type& key)
{
	const int intValue(key->value());
	EnsureIndexInRange(intValue);
	return _storage[intValue];
}

void ScriptArray::RemoveMember(const key_type& key)
{
	const int intValue(key->value());
	EnsureIndexInRange(intValue);
	_storage.erase(_storage.begin() + intValue);
}

void ScriptArray::Clear()
{
	_storage.clear();
}

std::shared_ptr<ScriptDec> ScriptArray::Size() const
{
	return std::make_shared<ScriptDec>(int(_storage.size()));
}

ScriptArray::value_type ScriptArray::Push(const value_type& value)
{
	_storage.push_back(value->Clone());
	return _storage.back();
}

ScriptArray::value_type ScriptArray::Pop()
{
	value_type poppedElement = std::make_shared<value_type::element_type>(*_storage.back());
	_storage.pop_back();
	return poppedElement;
}

std::shared_ptr<BaseScriptVariable> ScriptArray::Clone() const
{
	return std::make_shared<ScriptArray>(*this);
}

ScriptArray::ScriptArray(bool isConst) : BaseScriptCollection<ScriptDec, BaseScriptVariable>(ScriptVariableType::Array, isConst)
{
}

ScriptArray::ScriptArray(const ScriptArray& scriptArray) : BaseScriptCollection<ScriptDec, BaseScriptVariable>(ScriptVariableType::Array, scriptArray.isConst())
{
	for(auto& iter : scriptArray._storage)
	{
		_storage.push_back(iter->Clone());
	}

	_const = false;
}
