#include "ScriptLinesView.h"

std::string ScriptLinesView::operator[](const int &index) const
{
	return at(index);
}

std::string ScriptLinesView::at(int index) const
{
	return _lines->at(_front + index);
}

std::vector<std::string>::iterator ScriptLinesView::begin() const
{
	return _lines->begin() + _front;
}

std::vector<std::string>::iterator ScriptLinesView::end() const
{
	return _lines->begin() + _back + 1;
}

std::vector<std::string>* ScriptLinesView::lines() const
{
	return _lines;
}

int ScriptLinesView::front() const
{
	return _front;
}

int ScriptLinesView::back() const
{
	return _back;
}

size_t ScriptLinesView::size() const
{
	return _back - _front;
}

ScriptLinesView::ScriptLinesView(std::vector<std::string>* lines)
{
	_lines = lines;
	_front = 0;
	_back = lines->size();
}

ScriptLinesView::ScriptLinesView(std::vector<std::string>* lines, int front, int back)
{
	if(front > back)
	{
		throw std::invalid_argument("Invalid ScriptLinesView range.");
	}

	_lines = lines;
	_front = front;
	_back = back;
}

ScriptLinesView::ScriptLinesView(const ScriptLinesView & lines, int front, int back)
{
	_lines = lines.lines();
	_front = lines.front() + front;
	_back = lines.front() + back;
}


ScriptLinesView::~ScriptLinesView() {}
