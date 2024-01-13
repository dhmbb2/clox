#pragma once

#include <iostream>
#include <string>

namespace loxcc {

void error(int line, const std::string& message);

void report(int line, const std::string& message, const std::string& where);

}