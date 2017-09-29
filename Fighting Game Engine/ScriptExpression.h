#pragma once
#include <vector>
#include <memory>

class ScriptBlock;
class BaseScriptVariable;

class ScriptExpression
{
	enum class State;

	ScriptBlock* _parent;
public:

	std::shared_ptr<BaseScriptVariable> Evaluate();
	ScriptExpression();
	~ScriptExpression();
};

