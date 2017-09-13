#pragma once
#include <string>
#include <vector>

class ScriptLinesView;
struct ScriptFunctionSignature;

enum class ScriptTokenType
{
	Invalid,

	Whitespace,
	Keyword,

	NumericLiteral,
	StringLiteral,

	ParenthesisGroup,

	Separator,
	Specifier,
	Operator
};

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

struct ScriptToken
{
	std::string token;
	ScriptTokenType type;
};

namespace ScriptParsingUtils
{
	int GetIndentationLevel(const std::string& line);
	int GetIndentationLevel(const std::string& line, unsigned int& out_lineStart);

	std::string TrimLine(const std::string& line, int& out_indentationLevel);

	int FindBlockEnd(const ScriptLinesView &lines, unsigned int blockStart);

	ScriptTokenType GetTokenType(char character);
	ScriptTokenType GetNextTokenType(const std::string& line, size_t startIndex, int& out_endIndex);

	void ParseLineTokens(const std::string& line, std::vector<ScriptToken> &out_tokens);

	ScriptFunctionSignature ParseFunctionSignature(const ScriptLinesView &lines, int declarationLine);
}
