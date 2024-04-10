#pragma once
#include "interpreter.h"

namespace clox {

// Run a file
void runFile(const char* path);

// Run a REPL
void runPrompt();

// Run a line
void run(const std::string& source, Interpreter& Interpreter);

}