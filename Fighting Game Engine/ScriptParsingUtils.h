#pragma once
#include <string>
#include <vector>
#include "ScriptToken.h"

class ScriptLinesView;
struct ScriptFunctionSignature;

enum class ScriptControlFlag
{
	None,

	Break,
	Continue,

	Return,
};

enum class ScriptLineType
{
	Invalid,
	Expression,
	FunctionDeclaration,
	LoopDeclaration,
	ConditionalDeclaration
};

namespace ScriptParsingUtils
{
	inline int GetIndentationLevel(const std::string& line);
	int GetIndentationLevel(const std::string& line, unsigned int& out_lineStart);

	inline std::string TrimLine(const std::string& line);
	std::string TrimLine(const std::string& line, int& out_indentationLevel);

	int FindBlockEnd(const ScriptLinesView& lines, unsigned int blockStart);

	ScriptTokenType GetTokenType(char character);
	ScriptTokenType GetNextTokenType(const std::string& line, size_t startIndex, int& out_endIndex);

	void ParseLineTokens(const std::string& line, std::vector<ScriptToken>& out_tokens);

	ScriptFunctionSignature ParseFunctionSignature(const ScriptLinesView& lines, int declarationLine);

	void ParseConditionalExpression(const ScriptLinesView& lines, const std::vector<ScriptToken>& lineTokens, int cursor, std::vector<ScriptToken>& out_conditionalTokens, int& out_blockEnd);
}
