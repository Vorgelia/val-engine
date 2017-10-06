#pragma once
#include <string>
#include <vector>
#include "ScriptLine.h"

class ScriptLinesView
{
	std::vector<ScriptLine> *_lines;
	int _front;
	int _back;

public:
	ScriptLine operator[](std::vector<ScriptLine>::size_type index) const;

	ScriptLine at(std::vector<ScriptLine>::size_type index) const;

	std::vector<ScriptLine>::iterator begin() const;
	std::vector<ScriptLine>::iterator end() const;

	std::vector<ScriptLine>::size_type size() const;

	std::vector<ScriptLine>* lines() const;
	int front() const;
	int back() const;

	ScriptLinesView(std::vector<ScriptLine>* lines);
	ScriptLinesView(std::vector<ScriptLine>* lines, int front, int back);
	ScriptLinesView(const ScriptLinesView& lines, int front, int back);
	~ScriptLinesView();
};
