#pragma once

#include <unordered_map>
#include <stack>
#include "value.h"
#include "token.h"

namespace clox {

class Environment {
public:
  Environment() = default;

  Value get(Token);
  void define(Token, Value);
  void assign(Token, Value);


  std::unordered_map<std::string, Value> values;
};

class EnvironmentStackPopper {
  public:
  EnvironmentStackPopper(std::stack<Environment>& environments): _environments(environments) {
    _environments.push(Environment());
  }
  ~EnvironmentStackPopper() {
    _environments.pop();
  }

  std::stack<Environment>& _environments;
};

} // namespace clox
