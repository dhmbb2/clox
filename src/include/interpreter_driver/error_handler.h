#pragma once

#include <iostream>
#include <string>
#include "token.h"
#include "runtime_error.h"

namespace clox {

void error(Token token, const std::string& message);
void error(int line, const std::string& message);
void runtime_error(const RuntimeError& error);
void report(int line, const std::string& message, const std::string& where);

}