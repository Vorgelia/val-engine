#include "ScriptToken.h"

const std::string ScriptToken::preprocessor_pragma = "pragma";

const std::string ScriptToken::function_declaration = "function";
const std::string ScriptToken::conditional_declaration = "if";
const std::string ScriptToken::conditional_else = "else";
const std::string ScriptToken::conditional_elseif = "elseif";
const std::string ScriptToken::while_loop_declaration = "while";
const std::string ScriptToken::ranged_switch_declaration = "switch";

const std::string ScriptToken::block_end = "end";
const std::string ScriptToken::block_break = "break";
const std::string ScriptToken::block_continue = "continue";
const std::string ScriptToken::block_return = "return";

const std::string ScriptToken::switch_case = "case";
const std::string ScriptToken::switch_default = "default";

const std::string ScriptToken::value_null = "null";
const std::string ScriptToken::value_true = "true";
const std::string ScriptToken::value_false = "false";

const std::string ScriptToken::type_int = "int";
const std::string ScriptToken::type_bool = "bool";
const std::string ScriptToken::type_string = "string";
const std::string ScriptToken::type_collection = "collection";