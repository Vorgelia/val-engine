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
	
	std::shared_ptr<BaseScriptVariable> RunFunction(std::string name, const std::vector<std::shared_ptr<BaseScriptVariable>> &variables) override;

	ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner);
	~ScriptParentBlock();
};

