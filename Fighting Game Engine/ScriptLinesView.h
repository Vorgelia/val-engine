#pragma once
#include <string>
#include <vector>

class ScriptLinesView
{
	std::vector<std::string> *_lines;
	int _front;
	int _back;

public:
	std::string operator[](const int &index);

	std::string at(int index);

	std::vector<std::string>::iterator begin();
	std::vector<std::string>::iterator end();

	size_t size();

	std::vector<std::string>* lines();
	int front();
	int back();

	ScriptLinesView(std::vector<std::string>* lines);
	ScriptLinesView(std::vector<std::string>* lines, int front, int back);
	~ScriptLinesView();
};
