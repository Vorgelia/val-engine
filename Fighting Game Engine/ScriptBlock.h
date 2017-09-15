#pragma once
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>
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

	std::map<std::string, ScriptVariable> _variables;

	void ParseLine(const std::string &line);

	virtual void HandleExpressionLine(std::vector<ScriptToken> &tokens);
	virtual void HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens);
	virtual void HandleLoopDeclarationLine(std::vector<ScriptToken> &tokens);
	virtual void HandleConditionalDeclarationLine(std::vector<ScriptToken> &tokens);

	virtual bool HandleControlFlag();
public:
	size_t cursor(bool absolute = true);

	virtual void Run();

	virtual void RunFunction(std::string name, const std::vector<ScriptVariable> &variables);
	ScriptVariable GetVariable(std::string name);

	ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

