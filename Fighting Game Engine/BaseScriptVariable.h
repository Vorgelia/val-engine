#pragma once
#include<string>

enum class ScriptVariableType
{
	Error = 0,
	Bool = 1,
	Int = 2,
	String = 3
};

enum class ScriptOperatorType;

class BaseScriptVariable
{
protected:
	ScriptVariableType _type;
	bool _const;
	bool _initialized;
public:
	ScriptVariableType type() const;
	bool isConst() const;

	BaseScriptVariable(ScriptVariableType type, bool isConst = false);
	BaseScriptVariable();
	~BaseScriptVariable();
};