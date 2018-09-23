#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ScriptLinesView.h"
#include "BaseScriptVariable.h"

#define VE_SCRIPT_RETURN_VARIABLE_ID "__VE_RETURN"

class Script;
enum class ScriptLineType;
struct ScriptToken;
struct ScriptLine;

class ScriptBlock
{
	friend class ScriptExpression;
protected:
	Script* _owner;
	ScriptBlock* _parent;

	ScriptLinesView _lines;
	size_t _cursor;
	int _depth;

	std::unordered_map<std::string, std::shared_ptr<BaseScriptVariable>> _variables;

	virtual void ParseLine(ScriptLine &line);

	virtual void HandleExpressionLine(std::vector<ScriptToken>& tokens);
	virtual void HandleFunctionDeclarationLine(std::vector<ScriptToken>& tokens);
	virtual void HandleLoopDeclarationLine(std::vector<ScriptToken>& tokens, int& out_blockEnd);
	virtual void HandleConditionalDeclarationLine(std::vector<ScriptToken>& tokens, int& out_blockEnd);
	virtual void HandleRangedSwitchDeclarationLine(std::vector<ScriptToken>& tokens, int& out_blockEnd);

	std::shared_ptr<BaseScriptVariable> EvaluateExpression(std::vector<ScriptToken>& tokens);

	virtual bool HandleControlFlag();
public:
	size_t cursor(bool absolute = true) const;

	virtual void Run();

	virtual std::shared_ptr<BaseScriptVariable> RunFunction(std::string name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables);

	void AddVariable(std::string name, std::shared_ptr<BaseScriptVariable> variable, bool allowReplace = false);
	std::shared_ptr<BaseScriptVariable> GetVariable(std::string name);

	ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	virtual ~ScriptBlock();
};

