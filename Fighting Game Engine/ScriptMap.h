#pragma once
#include "BaseScriptVariable.h"
#include <unordered_map>
#include <string>
#include <memory>
#include "ScriptVariable.h"
#include "BaseScriptCollection.h"

class IReflectable;

class ScriptMap : public BaseScriptCollection<ScriptString, BaseScriptVariable, ScriptVariableType::Map>
{
private:
	typedef BaseScriptCollection<ScriptString, BaseScriptVariable, ScriptVariableType::Map> Super;

	std::unordered_map<key_type::value_type, std::shared_ptr<value_type>> _storage;

public:
	std::unordered_map<key_type::value_type, std::shared_ptr<value_type>> storage() const { return _storage; }

	std::shared_ptr<value_type> AddMember(const std::shared_ptr<key_type>& key, const std::shared_ptr<value_type>& value);
	std::shared_ptr<value_type> GetMember(const std::shared_ptr<key_type>& key) override;
	void RemoveMember(const std::shared_ptr<key_type>& key) override;
	void Clear() override;

	json ToJSON() const override;
	std::shared_ptr<BaseScriptVariable> Clone() const override;

	explicit ScriptMap(bool isConst = false);
	explicit ScriptMap(const json& j);
	explicit ScriptMap(const ScriptMap& map);
	~ScriptMap() = default;
};
