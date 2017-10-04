#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "ScriptLinesView.h"
#include "BaseScriptVariable.h"

#define VE_SCRIPT_RETURN_VARIABLE_ID "__VE_RETURN"

class Script;
enum class ScriptLineType;
struct ScriptToken;

class ScriptBlock
{
	friend class ScriptExpression;
protected:
	Script* _owner;
	ScriptBlock* _parent;

	ScriptLinesView _lines;
	size_t _cursor;
	int _depth;

	std::map<std::string, std::shared_ptr<BaseScriptVariable>> _variables;

	void ParseLine(const std::string &line);

	virtual void HandleExpressionLine(std::vector<ScriptToken> &tokens);
	virtual void HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens);
	virtual void HandleLoopDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd);
	virtual void HandleConditionalDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd);

	std::shared_ptr<BaseScriptVariable> EvaluateExpression(std::vector<ScriptToken>& tokens);

	virtual bool HandleControlFlag();
public:
	size_t cursor(bool absolute = true);

	virtual void Run();

	virtual std::shared_ptr<BaseScriptVariable> RunFunction(std::string name, const std::vector<std::shared_ptr<BaseScriptVariable>> &variables);

	void AddVariable(std::string name, std::shared_ptr<BaseScriptVariable> variable);
	std::shared_ptr<BaseScriptVariable> GetVariable(std::string name);

	ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

