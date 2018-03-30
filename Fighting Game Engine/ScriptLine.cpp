#include "ScriptLine.h"
#include <utility>

ScriptLine::ScriptLine(std::string line, int index, int indentationLevel, std::vector<ScriptToken>& tokens)
	: line(std::move(line))
	, index(index)
	, depth(indentationLevel)
	, tokens(tokens)
{
}

ScriptLine::~ScriptLine()
= default;
