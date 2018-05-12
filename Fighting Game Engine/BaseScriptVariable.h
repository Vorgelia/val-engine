#pragma once
#include <string>
#include <unordered_map>
#include <memory>

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

enum class ScriptOperatorType;

class BaseScriptVariable
{
	friend class ScriptManager;
protected:
	ScriptVariableType _type;
	bool _const;
	bool _initialized;

	static const std::unordered_map<std::string, ScriptVariableType> variableTypeLookup;
public:

	static ScriptVariableType GetVariableType(const std::string& token);

	ScriptVariableType type() const;
	bool isConst() const;
	bool isInitialized() const;
	virtual std::string ToString();
	virtual std::shared_ptr<BaseScriptVariable> Clone() const;

	BaseScriptVariable(ScriptVariableType type, bool isConst = false);
	BaseScriptVariable();
	virtual ~BaseScriptVariable();
};