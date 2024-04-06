#include <string>
#include <iostream>
#include <format>
#include "interpreter_driver/initializer.h"
#include "interpreter_driver/error_handler.h"
#include "token.h"

namespace clox{

extern bool had_error;

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

void report(int line, const std::string& message, const std::string& where) {
    std::string error_string = std::format("Error: {} \n line {}: at {}", message, line, where);
    std::cerr << error_string << '\n';
}

}