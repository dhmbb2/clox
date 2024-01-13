#include <string>
#include <iostream>
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
    std::cerr << line << message << where;
}

}