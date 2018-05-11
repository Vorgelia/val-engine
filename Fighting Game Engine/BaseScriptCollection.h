#pragma once
#include "BaseScriptVariable.h"
#include <memory>

template<typename KeyT, typename ValueT>
class BaseScriptCollection : public BaseScriptVariable
{
public:
	typedef std::shared_ptr<KeyT> key_type;
	typedef std::shared_ptr<ValueT> value_type;

	virtual value_type GetMember(const key_type& key) = 0;
	virtual void RemoveMember(const key_type& key) = 0;

	virtual void Clear() = 0;

	BaseScriptCollection(ScriptVariableType type, bool isConst = false) : BaseScriptVariable(type, isConst) { }
	virtual ~BaseScriptCollection() = default;
};