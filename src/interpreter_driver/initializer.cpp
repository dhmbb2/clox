#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "interpreter_driver/initializer.h"
#include "scanner.h"
#include "token.h"
#include "parser.h"
#include "printer.h"
#include "interpreter.h"

namespace clox {

bool had_error = false;
bool had_runtime_error = false;

void runFile(const char* path) {
    // use istreambuf_iterator to read file into string
    std::ifstream script_file(path);
    if (!script_file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(74);
    }
    const std::string source((std::istreambuf_iterator<char>(script_file)),
                        std::istreambuf_iterator<char>());

    // run the script
    run(source);
    script_file.close();
}

void runPrompt() {
    std::string line;
    while (true) {
        std::cout << ">> ";
        std::getline(std::cin, line);
        if (line == "exit") {
            break;
        }
        run(line);
    }
}

void run(const std::string& source) {
    Scanner scanner(source);
    scanner.scanTokens();
    Parser parser(scanner);
    Interpreter interpreter;
    Printer printer;
    auto expr = parser.parser();

    interpreter.interpret(*expr.get());
    if (had_error) {had_error = 0; return;}
    if (had_runtime_error) {had_runtime_error = 0; return;}
}

}