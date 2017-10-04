#pragma once
#include <vector>
#include <memory>

class ScriptBlock;
class BaseScriptVariable;
struct ScriptToken;

class ScriptExpression
{
	enum class State;

	ScriptBlock* _parent;
	const std::vector<ScriptToken>& _tokens;
public:

	std::shared_ptr<BaseScriptVariable> Evaluate();
	ScriptExpression(ScriptBlock* parent, const std::vector<ScriptToken>& tokens);
	~ScriptExpression();
};

