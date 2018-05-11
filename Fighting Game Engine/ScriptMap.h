#pragma once
#include "BaseScriptVariable.h"
#include <unordered_map>
#include <string>
#include <memory>
#include "ScriptVariable.h"
#include "BaseScriptCollection.h"

class ScriptMap : public BaseScriptCollection<ScriptString, BaseScriptVariable>
{
	std::unordered_map<std::string, value_type> _storage;

public:
	value_type AddMember(const key_type& key, const value_type& value);
	value_type GetMember(const key_type& key) override;
	void RemoveMember(const key_type& key) override;
	void Clear() override;

	std::shared_ptr<BaseScriptVariable> Clone() const override;

	explicit ScriptMap(bool isConst = false);
	ScriptMap(const ScriptMap& map);
	~ScriptMap() = default;
};
