#pragma once
#include <string>
#include <vector>

class ScriptLinesView;

namespace ScriptParsingUtils
{
	int GetIndentationLevel(std::string line);
	int GetIndentationLevel(std::string line, unsigned int& lineStart);

	std::string TrimLine(std::string line, int& indentationLevel);
	int FindBlockEnd(const ScriptLinesView &lines, unsigned int blockStart);
}
