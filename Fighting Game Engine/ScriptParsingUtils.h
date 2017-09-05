#pragma once
#include <string>
#include <vector>

class ScriptLinesView;
class ScriptFunctionSignature;

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

namespace ScriptParsingUtils
{
	int GetIndentationLevel(std::string line);
	int GetIndentationLevel(std::string line, unsigned int& lineStart);

	std::string TrimLine(std::string line, int& indentationLevel);

	int FindBlockEnd(const ScriptLinesView &lines, unsigned int blockStart);

	ScriptTokenType GetTokenType(char character);
	ScriptTokenType GetNextTokenType(std::string line, int startIndex, int& endIndex);

	ScriptFunctionSignature ParseFunctionSignature(const ScriptLinesView &lines, int declarationLine);
}
