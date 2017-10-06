#include "ScriptLine.h"

ScriptLine::ScriptLine(std::string line, int index, int indentationLevel, std::vector<ScriptToken>& tokens)
	:line(line)
	, index(index)
	, depth(indentationLevel)
	, tokens(tokens)
{
}


ScriptLine::~ScriptLine()
{
}
