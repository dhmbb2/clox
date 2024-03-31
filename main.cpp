#include <iostream>
#include "interpreter_driver/initializer.h"


int main (int argc, char** argv) {
    if (argc > 2) {
        std::cout << "Usage: lox [script]" << std::endl;
        exit(64);
    } else if (argc == 2) {
        std::cout << "Running file: " << argv[1] << std::endl;
        clox::runFile(argv[1]);
    } else {
        std::cout << "Running REPL" << std::endl;
        clox::runPrompt();
    }
    return 0;
}
