#pragma once
#include <string>
#include <vector>

class ScriptLinesView
{
	std::vector<std::string> *_lines;
	int _front;
	int _back;

public:
	std::string operator[](const int &index) const;

	std::string at(int index) const;

	std::vector<std::string>::iterator begin() const;
	std::vector<std::string>::iterator end() const;

	size_t size() const;

	std::vector<std::string>* lines() const;
	int front() const;
	int back() const;

	ScriptLinesView(std::vector<std::string>* lines);
	ScriptLinesView(std::vector<std::string>* lines, int front, int back);
	ScriptLinesView(const ScriptLinesView& lines, int front, int back);
	~ScriptLinesView();
};
