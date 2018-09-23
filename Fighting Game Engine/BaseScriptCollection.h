#pragma once
#include "BaseScriptVariable.h"
#include <memory>

template<typename KeyT, typename ValueT, ScriptVariableType VariableT>
class BaseScriptCollection : public BaseTypedScriptVariable<VariableT>
{
public:
	typedef KeyT key_type;
	typedef ValueT value_type;
	
	virtual std::shared_ptr<value_type> GetMember(const std::shared_ptr<key_type>& key) = 0;
	virtual void RemoveMember(const std::shared_ptr<key_type>& key) = 0;

	virtual void Clear() = 0;
	std::string ToString() const override { return "Collection"; }

	BaseScriptCollection(bool isConst = false);
	explicit BaseScriptCollection(const json& j);
	virtual ~BaseScriptCollection() = default;
};

template <typename KeyT, typename ValueT, ScriptVariableType VariableT>
BaseScriptCollection<KeyT, ValueT, VariableT>::BaseScriptCollection(bool isConst)
	: BaseTypedScriptVariable<VariableT>(isConst)
{
	_initialized = true;
}

template <typename KeyT, typename ValueT, ScriptVariableType VariableT>
BaseScriptCollection<KeyT, ValueT, VariableT>::BaseScriptCollection(const json& j)
	: BaseTypedScriptVariable<VariableT>(j)
{
	
}
