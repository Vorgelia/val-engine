#pragma once
#include <stdarg.h>
#include <string>
#include <vector>
#include <map>
#include "ScriptLinesView.h"
#include "ScriptVariable.h"

class Script;
enum class ScriptLineType;

class ScriptBlock
{
protected:

	Script* _owner;
	ScriptBlock* _parent;

	int _depth;

	ScriptLinesView _lines;

	size_t _cursor;

	void ParseLine(const std::string &line);

	virtual void HandleExpressionLine(const std::vector<ScriptToken> &tokens);
	virtual void HandleFunctionDeclarationLine(const std::vector<ScriptToken> &tokens);
	virtual void HandleLoopDeclarationLine(const std::vector<ScriptToken> &tokens);
	virtual void HandleConditionalDeclarationLine(const std::vector<ScriptToken> &tokens);

	virtual bool HandleControlFlag();
public:
	size_t cursor();

	virtual void Run();

	ScriptBlock(ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptBlock();
};

