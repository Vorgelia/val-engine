#pragma once
#include "ScriptConditionalBlock.h"
#include "ScriptVariable.h"

class ScriptRangedSwitchBlock : public ScriptConditionalBlock
{
	bool _handledCaseLabel;
	std::shared_ptr<ScriptInt> _conditionResult;
public:

	void Run() override;
	bool HandleControlFlag() override;


	void ParseLine(ScriptLine &line) override;

	bool HandleConditionResult(std::shared_ptr<BaseScriptVariable> result) override;
	ScriptRangedSwitchBlock(std::vector<ScriptToken>& conditionTokens, ScriptLinesView lines, int depth, ScriptBlock* parent, Script* owner);
	~ScriptRangedSwitchBlock();
};

