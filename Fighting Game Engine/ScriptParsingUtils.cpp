#include "ScriptParsingUtils.h"
#define TAB_SPACE_AMOUNT 4



namespace ScriptParsingUtils
{

}

int ScriptParsingUtils::GetIndentationLevel(std::string line)
{
	unsigned int lineStart;
	return GetIndentationLevel(line, lineStart);
}

int ScriptParsingUtils::GetIndentationLevel(std::string line,unsigned int& lineStart)
{
	int indentationLevel = 0;
	int spaceCount = 0;

	lineStart = 0;

	for(int i = 0; i < line.length(); ++i)
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

	int j = line.length() - 1;
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

int ScriptParsingUtils::FindBlockEnd(std::vector<std::string> lines, unsigned int blockStart)
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