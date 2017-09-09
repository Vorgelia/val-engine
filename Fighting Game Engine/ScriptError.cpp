#include "ScriptError.h"

ScriptError::ScriptError(std::string message) :std::logic_error(message) {}