#include "ScriptParsingUtils.h"
#include "ScriptLinesView.h"
#include "ScriptFunctionSignature.h"
#include "ScriptError.h"
#include "CommonUtilIncludes.hpp"

#define TAB_SPACE_AMOUNT 4

namespace ScriptParsingUtils
{

}

int ScriptParsingUtils::GetIndentationLevel(std::string line)
{
	unsigned int lineStart;
	return GetIndentationLevel(line, lineStart);
}

int ScriptParsingUtils::GetIndentationLevel(std::string line, unsigned int& lineStart)
{
	int indentationLevel = 0;
	int spaceCount = 0;

	lineStart = 0;

	for(size_t i = 0; i < line.length(); ++i)
	{
		if(line[i] == '\t')
		{
			indentationLevel += 1;
		}
		else if(line[i] == ' ')
		{
			spaceCount += 1;
			if(spaceCount >= TAB_SPACE_AMOUNT)
			{
				spaceCount = 0;
				indentationLevel += 1;
			}
		}
		else
		{
			lineStart = i;
			return indentationLevel;
		}
	}

	return -1;
}

std::string ScriptParsingUtils::TrimLine(std::string line, int& indentationLevel)
{
	unsigned int i;
	indentationLevel = GetIndentationLevel(line, i);

	if(indentationLevel < 0)
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

	indentationLevel = -1;
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
		if(lineDepth == blockDepth && line == "end")
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
			break;
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

ScriptTokenType ScriptParsingUtils::GetNextTokenType(std::string line, int startIndex, int& endIndex)
{
	endIndex = -1;

	if(startIndex >= line.length())
	{
		return ScriptTokenType::Invalid;
	}

	ScriptTokenType type = GetTokenType(line[startIndex]);
	if(startIndex == line.length() - 1)
	{
		endIndex = startIndex + 1;
		return type;
	}

	switch(type)
	{
		case ScriptTokenType::Invalid:
			break;
		case ScriptTokenType::ParenthesisGroup:
		{
												  int parenthesisDepth = 0;
												  for(int i = startIndex; i < line.length(); ++i)
												  {
													  if(line[i] == '(')
													  {
														  ++parenthesisDepth;
													  }
													  else if(line[i] == ')')
													  {
														  if(--parenthesisDepth == 0)
														  {
															  endIndex = i;
															  break;
														  }
													  }
												  }
		}
			break;
		case ScriptTokenType::StringLiteral:
		{
											   bool escapeCharacter = false;
											   for(int i = startIndex; i < line.length(); ++i)
											   {
												   if(!escapeCharacter)
												   {
													   if(line[i] == '\\')
													   {
														   escapeCharacter = true;
													   }
													   else if(line[i] == '"')
													   {
														   endIndex = i;
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
			endIndex = startIndex;
			break;
		case ScriptTokenType::Operator:
			for(int i = startIndex; i < line.length(); ++i)
			{
				if(GetTokenType(line[i]) == ScriptTokenType::Operator)
				{
					endIndex = i;
				}
				else
				{
					break;
				}
			}
			break;
		case ScriptTokenType::NumericLiteral:
			for(int i = startIndex; i < line.length(); ++i)
			{
				if(isalpha(i))
				{
					endIndex = i;
					break;
				}
				else if(!isdigit(i))
				{
					endIndex = i - 1;
					break;
				}
			}
			break;
		case ScriptTokenType::Keyword:
			for(int i = startIndex; i < line.length(); ++i)
			{
				if(isalnum(line[i]) || line[i] == '_')
				{
					endIndex = i;
				}
				else
				{
					break;
				}
			}
			break;
		case ScriptTokenType::Whitespace:
			for(int i = startIndex; i < line.length(); ++i)
			{
				if(GetTokenType(line[i]) == ScriptTokenType::Whitespace)
				{
					endIndex = i;
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

	int cursor = 0;
	int state = 0;
	while(cursor < line.length())
	{
		int endIndex;
		ScriptTokenType type = GetNextTokenType(line, cursor, endIndex);
		if(endIndex < 0)
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
				if(token != "function")
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
				if(type != ScriptTokenType::ParenthesisGroup)
				{
					throw ScriptError("Parser error: Invalid function definition.");
				}
				if(token.length() < 2)
				{
					break;
				}
				token = token.substr(1, token.length() - 2);
				boost::split(signature.arguments, token, boost::is_any_of(","), boost::token_compress_on);
				break;
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
				signature.returnType = token;
		}
	}

	return signature;
}