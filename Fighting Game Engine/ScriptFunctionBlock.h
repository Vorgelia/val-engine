#pragma once
#include "ScriptBlock.h"

struct ScriptFunctionSignature;

class ScriptFunctionBlock :
	public ScriptBlock
{
	ScriptFunctionSignature* _signature;

	bool HandleControlFlag() override;
public:
	std::shared_ptr<BaseScriptVariable> returnValue() const;

	void Run(const std::vector<std::shared_ptr<BaseScriptVariable>> &variables);

	ScriptFunctionBlock(ScriptFunctionSignature* signature, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptFunctionBlock();
};

