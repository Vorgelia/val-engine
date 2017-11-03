#include "ScriptCollection.h"
#include "ScriptError.h"

std::shared_ptr<BaseScriptVariable> ScriptCollection::AddMember(std::string name, std::shared_ptr<BaseScriptVariable> var)
{
	return _memberLookup.emplace(std::make_pair(name, var)).first->second;
}

std::shared_ptr<BaseScriptVariable> ScriptCollection::GetMember(std::string name)
{
	auto& iter = _memberLookup.find(name);
	if(iter == _memberLookup.end())
	{
		throw ScriptError("Attempting to index invalid member of collection ["+name+"]");
	}

	return iter->second;
}

ScriptCollection::ScriptCollection(const std::vector<std::pair<std::string, std::shared_ptr<BaseScriptVariable>>>& members, bool isConst) :
	BaseScriptVariable(ScriptVariableType::Collection, isConst)
{
	for(auto& iter : members)
	{
		_memberLookup.emplace(iter);
	}
}

ScriptCollection::ScriptCollection(bool isConst) :
	BaseScriptVariable(ScriptVariableType::Collection, isConst)
{

}

ScriptCollection::~ScriptCollection()
{
}
