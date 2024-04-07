cmake_minimum_required(VERSION 3.22)
project(clox)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Debug")

include_directories(src/include)
include_directories(src/include/interpreter_driver)

add_executable(main
        src/include/interpreter_driver/error_handler.h
        src/include/interpreter_driver/initializer.h
        src/include/scanner.h
        src/include/token.h
        src/include/Expr.h
        src/include/parser.h
        src/include/printer.h
        src/include/interpreter.h
        src/include/runtime_error.h
        src/interpreter_driver/error_handler.cpp
        src/interpreter_driver/initializer.cpp
        src/scanner.cpp
        src/token.cpp
        src/parser.cpp
        src/interpreter.cpp
        main.cpp)
