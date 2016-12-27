#pragma once
#include<string>
#include<vector>
class LogItem{
public:
	std::string message;
	std::vector<std::string> stack;
	std::string ToString(int stackAmount = 100);
	LogItem(std::string msg, std::vector<std::string> stack);
	LogItem(std::string msg);
};
