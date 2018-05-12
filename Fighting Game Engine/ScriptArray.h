#pragma once
#include "BaseScriptCollection.h"
#include <vector>
#include "ScriptVariable.h"

class ScriptArray : public BaseScriptCollection<ScriptDec, BaseScriptVariable>
{
private:
	std::vector<value_type> _storage;

	void EnsureIndexInRange(int index) const;

public:
	value_type GetMember(const key_type& key) override;
	void RemoveMember(const key_type& key) override;

	value_type Back();
	
	void Clear() override;
	
	std::shared_ptr<ScriptDec> Size() const;
	value_type Push(const value_type& value);
	value_type Pop();

	std::shared_ptr<BaseScriptVariable> Clone() const override;

	explicit ScriptArray(bool isConst = false);
	ScriptArray(const ScriptArray& scriptArray);
	~ScriptArray() = default;
	
};