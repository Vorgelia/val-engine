#include "ScriptParsingUtils.h"
#include "ScriptLinesView.h"
#include "ScriptFunctionSignature.h"
#include "ScriptError.h"
#include "CommonUtilIncludes.hpp"
#include "BaseScriptVariable.h"

#define VE_TAB_SPACE_AMOUNT 4

namespace ScriptParsingUtils
{

}

inline int ScriptParsingUtils::GetIndentationLevel(const std::string& line)
{
	unsigned int lineStart;
	return GetIndentationLevel(line, lineStart);
}

int ScriptParsingUtils::GetIndentationLevel(const std::string& line, unsigned int& out_lineStart)
{
	int indentationLevel = 0;
	int spaceCount = 0;

	out_lineStart = 0;

	for(size_t i = 0; i < line.length(); ++i)
	{
		if(line[i] == '\t')
		{
			indentationLevel += 1;
		}
		else if(line[i] == ' ')
		{
			spaceCount += 1;
			if(spaceCount >= VE_TAB_SPACE_AMOUNT)
			{
				spaceCount = 0;
				indentationLevel += 1;
			}
		}
		else
		{
			out_lineStart = i;
			return indentationLevel;
		}

		if(i == line.length() - 1)
		{
			return -1;
		}
	}

	return -1;
}

std::string ScriptParsingUtils::TrimLine(const std::string& line)
{
	int indentation = 0;
	return TrimLine(line, indentation);
}

std::string ScriptParsingUtils::TrimLine(const std::string& line, int& out_indentationLevel)
{

	size_t lineStart;
	out_indentationLevel = GetIndentationLevel(line, lineStart);

	if(out_indentationLevel < 0)
	{
		return "";
	}

	size_t lineEnd = lineStart;

	for(size_t i = lineStart; i < line.length(); ++i)
	{
		if(i > 0 && line[i] == '/'&& line[i - 1] == '/')
		{
			if(i < 2)
			{
				out_indentationLevel = -1;
				return "";
			}
			lineEnd = i - 2;
			break;
		}
		lineEnd = i;
	}

	for(size_t i = lineEnd; i >= lineStart; --i)
	{
		if(line[i] != ' ' && line[i] != '\t')
		{
			return line.substr(lineStart, i - lineStart + 1);
		}
	}

	out_indentationLevel = -1;
	return "";
}

int ScriptParsingUtils::FindBlockEnd(const ScriptLinesView &lines, unsigned int blockStart, bool requireEndDeclaration)
{
	if(blockStart >= lines.size())
	{
		throw ScriptError("Improperly terminated block");
	}

	for(size_t i = blockStart + 1; i < lines.size(); i++)
	{
		if(lines[blockStart].depth == lines[i].depth && (!requireEndDeclaration || lines[i].line == ScriptToken::block_end))
		{
			return i - 1;
		}
	}

	throw ScriptError("Improperly terminated block");
}

ScriptTokenType ScriptParsingUtils::GetTokenType(char character)
{
	switch(character)
	{
	case ' ':
	case '\t':
		return ScriptTokenType::Whitespace;
	case '\"':
		return ScriptTokenType::StringLiteral;
	case '(':
		return ScriptTokenType::ParenthesisGroup;
	case '[':
		return ScriptTokenType::SquareBracketGroup;
	case ',':
		return ScriptTokenType::Separator;
	case ':':
		return ScriptTokenType::Specifier;
	case '+':
	case '-':
	case '|':
	case '&':
	case '=':
	case '*':
	case '/':
	case '>':
	case '<':
	case '!':
	case '~':
	case '%':
		return ScriptTokenType::Operator;
	case '#':
		return ScriptTokenType::Preprocessor;
	}

	if(isalpha(character))
	{
		return ScriptTokenType::Keyword;
	}
	else if(isdigit(character))
	{
		return ScriptTokenType::NumericLiteral;
	}

	return ScriptTokenType::Invalid;
}

ScriptTokenType ScriptParsingUtils::GetNextTokenType(const std::string& line, size_t startIndex, int& out_endIndex)
{
	out_endIndex = -1;

	if(startIndex >= line.length())
	{
		return ScriptTokenType::Invalid;
	}

	ScriptTokenType type = GetTokenType(line[startIndex]);
	if(startIndex == line.length() - 1)
	{
		out_endIndex = startIndex + 1;
		return type;
	}

	switch(type)
	{
	case ScriptTokenType::Invalid:
		break;
	case ScriptTokenType::ParenthesisGroup:
	{
		int parenthesisDepth = 0;
		for(size_t i = startIndex; i < line.length(); ++i)
		{
			if(line[i] == '(')
			{
				++parenthesisDepth;
			}
			else if(line[i] == ')')
			{
				if(--parenthesisDepth == 0)
				{
					out_endIndex = i;
					break;
				}
			}
		}
	}
	break;
	case ScriptTokenType::SquareBracketGroup:
	{
		int parenthesisDepth = 0;
		for(size_t i = startIndex; i < line.length(); ++i)
		{
			if(line[i] == '[')
			{
				++parenthesisDepth;
			}
			else if(line[i] == ']')
			{
				if(--parenthesisDepth == 0)
				{
					out_endIndex = i;
					break;
				}
			}
		}
	}
	break;
	case ScriptTokenType::StringLiteral:
	{
		bool escapeCharacter = false;
		for(size_t i = startIndex + 1; i < line.length(); ++i)
		{
			if(!escapeCharacter)
			{
				if(line[i] == '\\')
				{
					escapeCharacter = true;
				}
				else if(line[i] == '"')
				{
					out_endIndex = i;
					break;
				}
			}
			else
			{
				escapeCharacter = false;
			}
		}
	}
	break;
	case ScriptTokenType::Separator:
	case ScriptTokenType::Specifier:
	case ScriptTokenType::Preprocessor:
		out_endIndex = startIndex;
		break;
	case ScriptTokenType::Operator:
		for(size_t i = startIndex; i < line.length(); ++i)
		{
			if(GetTokenType(line[i]) == ScriptTokenType::Operator)
			{
				out_endIndex = i;
			}
			else
			{
				break;
			}
		}
		break;
	case ScriptTokenType::NumericLiteral:
		for(size_t i = startIndex; i < line.length(); ++i)
		{
			if(isdigit(line[i]) || line[i] == '.')
			{
				out_endIndex = i;
			}
			else
			{
				break;
			}
		}
		break;
	case ScriptTokenType::Keyword:
		for(size_t i = startIndex; i < line.length(); ++i)
		{
			if(isalnum(line[i]) || line[i] == '_')
			{
				out_endIndex = i;
			}
			else
			{
				break;
			}
		}
		break;
	case ScriptTokenType::Whitespace:
		for(size_t i = startIndex; i < line.length(); ++i)
		{
			if(GetTokenType(line[i]) == ScriptTokenType::Whitespace)
			{
				out_endIndex = i;
			}
			else
			{
				break;
			}
		}
		break;
	}
	return type;
}

void ScriptParsingUtils::ParseLineTokens(const std::string& line, std::vector<ScriptToken> &out_tokens)
{
	out_tokens.clear();
	ScriptTokenType tokenType;
	size_t cursor = 0;
	int endIndex;
	while(cursor < line.length())
	{
		tokenType = GetNextTokenType(line, cursor, endIndex);
		if(endIndex < 0 || tokenType == ScriptTokenType::Invalid)
		{
			return;
		}

		switch(tokenType)
		{
		case ScriptTokenType::ParenthesisGroup:
		case ScriptTokenType::SquareBracketGroup:
		case ScriptTokenType::StringLiteral:
			out_tokens.push_back(ScriptToken{
				line.substr(cursor + 1, endIndex - cursor - 1)
				, tokenType });
			break;
		case ScriptTokenType::Whitespace:
			break;
		default:
			out_tokens.push_back(ScriptToken{
				line.substr(cursor, endIndex - cursor + 1)
				, tokenType });
			break;
		}

		cursor = endIndex + 1;
	}
}

int ScriptParsingUtils::GetNextTokenOfType(ScriptTokenType type, const std::vector<ScriptToken>& tokens, int startIndex)
{
	for(size_t i = startIndex; i < tokens.size(); ++i)
	{
		if(tokens[i].type == type)
		{
			return i;
		}
	}
	return -1;
}

ScriptFunctionSignature ScriptParsingUtils::ParseFunctionSignature(const ScriptLinesView &lines, int declarationLine)
{
	ScriptFunctionSignature signature;

	if(lines[declarationLine].depth < 0)
	{
		throw ScriptError("Parser Error - Invalid function declaration line. Somehow the parser is completely broken.");
	}

	signature.end = FindBlockEnd(lines, declarationLine);
	if(signature.end < 0)
	{
		throw ScriptError("Parser error: Improperly terminated function '" + signature.name + "'.");
	}

	signature.start = declarationLine;

	std::vector<ScriptToken>& declLineTokens(lines[declarationLine].tokens);

	if(declLineTokens.size() != 5
		|| declLineTokens[0].token != ScriptToken::function_declaration)
	{
		throw ScriptError("Parser Error - Invalid function declaration.");
	}

	if(declLineTokens[1].type != ScriptTokenType::Keyword)
	{
		throw ScriptError("Parser Error - Invalid function declaration name.");
	}
	signature.name = declLineTokens[1].token;

	if(declLineTokens[2].type != ScriptTokenType::ParenthesisGroup)
	{
		throw ScriptError("Parser Error - Invalid function declaration arguments.");
	}
	ParseFunctionArgumentSignatures(declLineTokens[2].token, signature.arguments);

	if(declLineTokens[3].type != ScriptTokenType::Specifier)
	{
		throw ScriptError("Parser Error - Invalid return type specifier.");
	}

	if(declLineTokens[4].type != ScriptTokenType::Keyword || (signature.returnType = BaseScriptVariable::GetVariableType(declLineTokens[4].token)) == ScriptVariableType::Invalid)
	{
		throw ScriptError("Parser Error - Invalid function return type.");
	}

	return signature;
}

void ScriptParsingUtils::ParseFunctionArgumentSignatures(const std::string & token, std::vector<ScriptVariableSignature>& out_signatures)
{
	out_signatures.clear();
	if(token.length() == 0)
	{
		return;
	}

	std::vector<ScriptToken> parenthesisTokens;
	ParseLineTokens(token, parenthesisTokens);

	if(parenthesisTokens.size() % 3 != 2)
	{
		throw ScriptError("Invalid function definition arguments.");
	}

	for(size_t i = 0; i < parenthesisTokens.size(); i += 3)
	{
		const ScriptToken& typeToken = parenthesisTokens[i];

		ScriptVariableType variableType;
		if(typeToken.type != ScriptTokenType::Keyword || (variableType = BaseScriptVariable::GetVariableType(typeToken.token)) == ScriptVariableType::Invalid)
		{
			throw ScriptError("Invalid variable type declaration " + typeToken.token);
		}

		const ScriptToken& nameToken = parenthesisTokens[i + 1];
		if(typeToken.type != ScriptTokenType::Keyword)
		{
			throw ScriptError("Invalid variable name declaration.");
		}

		if(i + 3 < parenthesisTokens.size() && parenthesisTokens[i + 2].type != ScriptTokenType::Separator)
		{
			throw ScriptError("Unexpected token " + parenthesisTokens[i + 2].token);
		}

		out_signatures.emplace_back(variableType, nameToken.token);
	}
}

void ScriptParsingUtils::ParseConditionalExpression(const ScriptLinesView& lines, const std::vector<ScriptToken>& lineTokens, int cursor, std::vector<ScriptToken>& out_conditionalTokens, int& out_blockEnd)
{
	if(lineTokens.size() != 2)
	{
		throw ScriptError("Parse error: Invalid conditional declaration.");
	}

	if(lineTokens[1].type != ScriptTokenType::ParenthesisGroup)
	{
		throw ScriptError("Parse error: Unexpected token: " + lineTokens[1].token);
	}

	out_blockEnd = ScriptParsingUtils::FindBlockEnd(lines, cursor);

	out_conditionalTokens.clear();
	ScriptParsingUtils::ParseLineTokens(lineTokens[1].token, out_conditionalTokens);
}
