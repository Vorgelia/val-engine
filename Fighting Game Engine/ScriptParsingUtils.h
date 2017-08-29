#pragma once
#include <string>
#include <vector>

namespace ScriptParsingUtils
{
	int GetIndentationLevel(std::string line);
	int GetIndentationLevel(std::string line, unsigned int& lineStart);

	std::string TrimLine(std::string line, int& indentationLevel);
	int FindBlockEnd(std::vector<std::string> lines, unsigned int blockStart);
}
