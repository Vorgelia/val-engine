#pragma once
#include <string>

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

struct ScriptToken
{
#pragma region Constant Token Declarations
	static const std::string function_declaration;
	static const std::string conditional_declaration;
	static const std::string conditional_else;
	static const std::string while_loop_declaration;

	static const std::string block_end;
	static const std::string block_break;
	static const std::string block_continue;
	static const std::string block_return;

	static const std::string null_token;
#pragma endregion

	std::string token;
	ScriptTokenType type;
};