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
	SquareBracketGroup,

	Separator,
	Specifier,
	Operator,
	Preprocessor,
};

struct ScriptToken
{
#pragma region Constant Token Declarations

	static const std::string preprocessor_pragma;

	static const std::string function_declaration;
	static const std::string conditional_declaration;
	static const std::string conditional_elseif;
	static const std::string conditional_else;
	static const std::string while_loop_declaration;
	static const std::string ranged_switch_declaration;

	static const std::string block_end;
	static const std::string block_break;
	static const std::string block_continue;
	static const std::string block_return;

	static const std::string switch_case;
	static const std::string switch_default;

	static const std::string value_null;
	static const std::string value_true;
	static const std::string value_false;

	static const std::string type_int;
	static const std::string type_bool;
	static const std::string type_string;
	static const std::string type_collection;
	static const std::string type_void;

#pragma endregion

	std::string token;
	ScriptTokenType type;
};