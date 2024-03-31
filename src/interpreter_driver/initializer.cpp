#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "interpreter_driver/initializer.h"
#include "scanner.h"
#include "token.h"

namespace loxcc {

bool had_error = false;

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
    for (const auto& token : scanner._tokens) {
        std::cout << Token::toString(token) << std::endl;
    }
}

}