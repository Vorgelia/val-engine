#include "ScriptError.h"

ScriptError::ScriptError(std::string message) : std::logic_error(message)
{
	_line = -1;
	_scriptName == "null";
}

ScriptError::ScriptError(std::string message, std::string scriptName, int line) : std::logic_error(message)
{
	_scriptName = scriptName;
	_line = line;
}

const char* ScriptError::what() const
{
	return std::string("(" + _scriptName + ":" + std::to_string(_line) + ")"+ std::logic_error::what()).c_str();
}