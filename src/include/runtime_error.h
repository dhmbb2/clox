#pragma once
#include <stdexcept>
#include <string>
#include "token.h"

namespace clox
{

class RuntimeError : public std::exception {
public:
  RuntimeError(Token token, std::string message) : _token(token), _message(std::move(message)) {}

  const char* what() const noexcept override {
    return _message.c_str();
  }

  Token _token;
  std::string _message;
};

} // namespace clox
