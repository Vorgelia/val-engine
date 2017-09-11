#pragma once
#include "ScriptBlock.h"

struct ScriptFunctionSignature;

class ScriptParentBlock :
	public ScriptBlock
{
	std::map<std::string, ScriptFunctionSignature> _functions;

	void PreProcess();
	bool HandleControlFlag();

	void HandleFunctionDeclarationLine(const std::vector<ScriptToken> &tokens);
public:
	
	void RunFunction(std::string name);

	ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner);
	~ScriptParentBlock();
};

