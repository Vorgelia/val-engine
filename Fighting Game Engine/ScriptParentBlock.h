#pragma once
#include "ScriptBlock.h"

struct ScriptFunctionSignature;

class ScriptParentBlock :
	public ScriptBlock
{
	std::unordered_map<std::string, ScriptFunctionSignature> _functions;

	bool HandleControlFlag() override;

	void HandleFunctionDeclarationLine(std::vector<ScriptToken> &tokens) override;
public:
	
	bool HasFunction(std::string name);
	std::shared_ptr<BaseScriptVariable> RunFunction(std::string name, std::vector<std::shared_ptr<BaseScriptVariable>> &variables) override;

	ScriptParentBlock(ScriptLinesView lines, int depth, Script* owner);
	~ScriptParentBlock();
};

