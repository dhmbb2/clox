#include <string>
#include <iostream>
#include <format>
#include "interpreter_driver/initializer.h"
#include "interpreter_driver/error_handler.h"
#include "token.h"
#include "runtime_error.h"

namespace clox{

extern bool had_error;
extern bool had_runtime_error;

void error(Token token, const std::string& message) {
    had_error = true;
    int line = token._line;
    std::string where = token._lexeme.value_or("empty string");
    if (token._type == TokenType::MYEOF)
        report(line, message, "at end");
    else
        report(line, message, where);
}

void error(int line, const std::string& message) {
    had_error = true;
    report(line, message, "");
}

void runtime_error(const RuntimeError& error) {
    std::cerr << std::format("Runtime Error:\n line[{}], {}\n", error._token._line, error.what());
    had_runtime_error = true;
}

void report(int line, const std::string& message, const std::string& where) {
    std::string error_string = std::format("Error: {} \n line {}: at {}", message, line, where);
    std::cerr << error_string << '\n';
}

}