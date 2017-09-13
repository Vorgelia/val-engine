#pragma once
#include "ScriptBlock.h"

struct ScriptFunctionSignature;

class ScriptParentBlock :
	public ScriptBlock
{
	std::map<std::string, ScriptFunctionSignature> _functions;

	bool HandleControlFlag();

	void HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens) override;
public:
	
	void RunFunction(std::string name) override;

	ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner);
	~ScriptParentBlock();
};

