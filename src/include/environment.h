#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include "value.h"
#include "token.h"

namespace clox {

class Environment {
public:
  Environment() = default;
  Environment(std::shared_ptr<Environment> enclosing): _enclosing(enclosing) {}

  Value get(Token);
  void define(Token, Value);
  void assign(Token, Value);

  std::unordered_map<std::string, Value> _values;
  std::shared_ptr<Environment> _enclosing;
};

} // namespace clox
