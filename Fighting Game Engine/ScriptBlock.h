#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include "ScriptLinesView.h"
#include "ScriptVariable.h"

class Script;
enum class ScriptLineType;
struct ScriptToken;

class ScriptBlock
{
protected:
	Script* _owner;
	ScriptBlock* _parent;

	ScriptLinesView _lines;
	size_t _cursor;
	int _depth;

	std::map<std::string, std::shared_ptr<ScriptVariable>> _variables;

	void ParseLine(const std::string &line);

	virtual void HandleExpressionLine(std::vector<ScriptToken> &tokens);
	virtual void HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens);
	virtual void HandleLoopDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd);
	virtual bool HandleConditionalDeclarationLine(std::vector<ScriptToken> &tokens, int& out_blockEnd);

	std::shared_ptr<ScriptVariable> EvaluateExpression(std::vector<ScriptToken>& tokens);

	virtual bool HandleControlFlag();
public:
	size_t cursor(bool absolute = true);

	virtual void Run();

	virtual std::shared_ptr<ScriptVariable> RunFunction(std::string name, const std::vector<std::shared_ptr<ScriptVariable>> &variables);
	std::shared_ptr<ScriptVariable> GetVariable(std::string name);

	ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

