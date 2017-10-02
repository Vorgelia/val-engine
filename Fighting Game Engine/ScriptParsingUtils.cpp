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
	unsigned int i;
	out_indentationLevel = GetIndentationLevel(line, i);

	if(out_indentationLevel < 0)
	{
		return "";
	}

	unsigned int j = line.length() - 1;

	for(; j > i; --j)
	{
		if(line[j] != ' ' && line[j] != '\t')
		{
			return line.substr(i, j - i + 1);
		}
	}

	out_indentationLevel = -1;
	return "";
}

int ScriptParsingUtils::FindBlockEnd(const ScriptLinesView &lines, unsigned int blockStart)
{
	if(blockStart >= lines.size())
	{
		return -1;
	}

	int blockDepth = GetIndentationLevel(lines[blockStart]);

	for(size_t i = blockStart + 1; i < lines.size(); i++)
	{
		int lineDepth;
		std::string line = ScriptParsingUtils::TrimLine(lines[i], lineDepth);
		if(lineDepth == blockDepth && line == ScriptToken::block_end)
		{
			return i;
		}
	}

	return -1;
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
		return ScriptTokenType::Operator;
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
	case ScriptTokenType::StringLiteral:
	{
		bool escapeCharacter = false;
		for(size_t i = startIndex; i < line.length(); ++i)
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
			if(!isdigit(i))
			{
				out_endIndex = i - 1;
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

		if(tokenType != ScriptTokenType::Whitespace)
		{
			out_tokens.push_back(ScriptToken{
				line.substr(cursor, endIndex - cursor + 1)
				, tokenType });
		}
		cursor = endIndex + 1;
	}
}

ScriptFunctionSignature ScriptParsingUtils::ParseFunctionSignature(const ScriptLinesView &lines, int declarationLine)
{
	ScriptFunctionSignature signature;

	signature.end = FindBlockEnd(lines, declarationLine);
	if(signature.end < 0)
	{
		return signature;
	}

	signature.start = declarationLine;

	int il;
	std::string line = TrimLine(lines[declarationLine], il);
	if(il < 0)
	{
		return signature;
	}

	size_t cursor = 0;
	int state = 0;
	while(cursor < line.length())
	{
		int endIndex;
		ScriptTokenType type = GetNextTokenType(line, cursor, endIndex);
		if(endIndex < 0 || type == ScriptTokenType::Invalid)
		{
			return signature;
		}

		std::string token = line.substr(cursor, endIndex - cursor + 1);
		cursor = endIndex + 1;
		if(type == ScriptTokenType::Whitespace)
		{
			continue;
		}

		switch(state++)
		{
		case 0:
			if(token != ScriptToken::function_declaration)
			{
				throw ScriptError("Parser error: Invalid function definition.");
			}
			break;
		case 1:
			if(type != ScriptTokenType::Keyword)
			{
				throw ScriptError("Parser error: Invalid function definition.");
			}
			signature.name = token;
			break;
		case 2:
		{
			if(type != ScriptTokenType::ParenthesisGroup)
			{
				throw ScriptError("Parser error: Invalid function definition.");
			}
			if(token.length() < 2)
			{
				break;
			}
			token = token.substr(1, token.length() - 2);
			std::vector<std::string> args;
			boost::split(args, token, boost::is_any_of(","), boost::token_compress_on);
			signature.arguments.reserve(args.size());
			for(std::string& arg : args)
			{
				int indentation;
				arg = TrimLine(arg, indentation);
				if(arg != "")
				{
					signature.arguments.push_back(arg);
				}
			}
			break;
		}
		case 3:
			if(type != ScriptTokenType::Specifier)
			{
				throw ScriptError("Parser error: Invalid function definition.");
			}
			break;
		case 4:
			if(type != ScriptTokenType::Keyword)
			{
				throw ScriptError("Parser error: Invalid function definition.");
			}

			if(token == ScriptToken::type_bool)
			{
				signature.returnType = ScriptVariableType::Bool;
			}
			else if(token == ScriptToken::type_int)
			{
				signature.returnType = ScriptVariableType::Int;
			}
			else if(token == ScriptToken::type_bool)
			{
				signature.returnType = ScriptVariableType::Bool;
			}
		}
	}

	return signature;
}

void ScriptParsingUtils::ParseConditionalExpression(const ScriptLinesView& lines , const std::vector<ScriptToken>& lineTokens, int cursor, std::vector<ScriptToken>& out_conditionalTokens, int& out_blockEnd)
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
