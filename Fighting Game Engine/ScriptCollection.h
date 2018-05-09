#pragma once
#include "BaseScriptVariable.h"
#include <unordered_map>
#include <string>
#include <memory>

class ScriptCollection :
	public BaseScriptVariable
{
	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _memberLookup;

public:
	std::shared_ptr<BaseScriptVariable> AddMember(const std::string& name, std::shared_ptr<BaseScriptVariable> var);
	std::shared_ptr<BaseScriptVariable> GetMember(const std::string& name);
	void RemoveMember(const std::string& name);

	ScriptCollection(const std::vector<std::pair<std::string, std::shared_ptr<BaseScriptVariable>>>& members, bool isConst = false);
	ScriptCollection(bool isConst = false);
	~ScriptCollection();
};