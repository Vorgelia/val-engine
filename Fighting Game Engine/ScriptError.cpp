#include "ScriptError.h"

ScriptError::ScriptError(const std::string& message) :std::logic_error(message) {}