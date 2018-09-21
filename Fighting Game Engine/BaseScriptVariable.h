#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "JSON.h"
#include "ScriptError.h"

enum class ScriptOperatorType;
class IReflectable;

enum class ScriptVariableType
{
	Invalid = -1,
	Null = 0,
	Bool = 1,
	Dec = 2,
	String = 3,
	Map = 4,
	Array = 5
};

class BaseScriptVariable
{
	friend class ScriptManager;

public:
	static const std::unordered_map<std::string, ScriptVariableType> variableTypeLookup;
	static ScriptVariableType GetVariableTypeFromToken(const std::string& token);

protected:
	bool _const;
	bool _initialized;

public:
	static const ScriptVariableType variable_type{ ScriptVariableType::Invalid };
	virtual ScriptVariableType type() const { return variable_type; };

	bool isConst() const;
	bool isInitialized() const;

	virtual std::string ToString() const;
	virtual json ToJSON() const;
	virtual std::shared_ptr<BaseScriptVariable> Clone() const;

	BaseScriptVariable(bool isConst = false);
	explicit BaseScriptVariable(const json& j);
	explicit BaseScriptVariable(const IReflectable& reflectable);
	virtual ~BaseScriptVariable() = default;
};

template<ScriptVariableType VariableType = ScriptVariableType::Invalid>
class BaseTypedScriptVariable : public BaseScriptVariable
{
public:
	static const ScriptVariableType variable_type{ VariableType };
	ScriptVariableType type() const override { return variable_type; }

	virtual json ToJSON() const override;

	BaseTypedScriptVariable(bool isConst = false) : BaseScriptVariable(isConst) {}
	BaseTypedScriptVariable(const json& j);
	virtual ~BaseTypedScriptVariable() = default;
};

template <ScriptVariableType VariableType>
json BaseTypedScriptVariable<VariableType>::ToJSON() const
{
	json parentJson = BaseScriptVariable::ToJSON();
	parentJson.emplace("ve_type", int(variable_type));
	return parentJson;
}

template <ScriptVariableType VariableType>
BaseTypedScriptVariable<VariableType>::BaseTypedScriptVariable(const json& j)
	: BaseScriptVariable(j)
{
	int variableType;
	if(JSON::TryGetMember(j, "ve_type", variableType) && ScriptVariableType{ variableType } != variable_type)
	{
		throw ScriptError("Attempting to construct a variable with a JSON describing an invalid type.");
	}
}
