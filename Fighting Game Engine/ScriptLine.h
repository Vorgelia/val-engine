#pragma once
#include <string>
#include <vector>
#include "ScriptToken.h"

struct ScriptLine
{
	const std::string line;
	const int index;
	const int depth;

	std::vector<ScriptToken> tokens;

	ScriptLine(const std::string& line, int index, int indentationLevel, std::vector<ScriptToken>& tokens);
	~ScriptLine();
};