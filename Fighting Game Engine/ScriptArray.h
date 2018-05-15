#pragma once
#include "BaseScriptCollection.h"
#include <vector>
#include "ScriptVariable.h"

class ScriptArray : public BaseScriptCollection<ScriptDec, BaseScriptVariable, ScriptVariableType::Array>
{
private:
	typedef BaseScriptCollection<ScriptDec, BaseScriptVariable, ScriptVariableType::Array> Super;

	std::vector<std::shared_ptr<value_type>> _storage;

	void EnsureIndexInRange(int index) const;

public:
	std::vector<std::shared_ptr<value_type>> storage() const { return _storage; }

	std::shared_ptr<value_type> GetMember(const std::shared_ptr<key_type>& key) override;
	void RemoveMember(const std::shared_ptr<key_type>& key) override;

	std::shared_ptr<value_type> Back();
	
	void Clear() override;
	
	std::shared_ptr<ScriptDec> Size() const;
	std::shared_ptr<value_type> Push(const std::shared_ptr<value_type>& value);
	std::shared_ptr<value_type> Pop();

	json ToJSON() const override;
	std::shared_ptr<BaseScriptVariable> Clone() const override;

	explicit ScriptArray(bool isConst = false);
	ScriptArray(const json& j);
	ScriptArray(const ScriptArray& scriptArray);
	~ScriptArray() = default;
	
};