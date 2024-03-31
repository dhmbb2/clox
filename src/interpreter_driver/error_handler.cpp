#include <string>
#include <iostream>
#include <format>
#include "interpreter_driver/initializer.h"
#include "interpreter_driver/error_handler.h"

namespace loxcc{

extern bool had_error;

void error(int line, const std::string& message) {
    had_error = true;
    const std::string where;
    report(line, message, where);
}

void report(int line, const std::string& message, const std::string& where) {
    std::string error_string = std::format("Error: {} \n line {}: {}", message, line, where);
    std::cerr << error_string;
    exit(1);
}

}