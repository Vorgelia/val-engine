#pragma once
#include "ScriptBlock.h"

struct ScriptFunctionSignature;

class ScriptParentBlock :
	public ScriptBlock
{
	std::map<std::string, ScriptFunctionSignature> _functions;

	void PreProcess();
	bool HandleControlFlag();

	void HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens) override;
public:
	
	void RunFunction(std::string name);

	ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner);
	~ScriptParentBlock();
};

