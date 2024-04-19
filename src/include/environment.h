#pragma once

#include <unordered_map>
#include <vector>
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

class EnvironmentStack {
public:
  EnvironmentStack() {
    _environments.push_back(Environment());
  }

  Value get(Token);
  void define(Token, Value);
  void assign(Token, Value);

  std::vector<Environment> _environments;
};

// RAII wrapper to prevent exit block from trap
class EnvironmentStackPopper {
  public:
  EnvironmentStackPopper(std::vector<Environment>& environments): _environments(environments) {
    _environments.push_back(Environment());
  }
  ~EnvironmentStackPopper() {
    _environments.pop_back();
  }

  std::vector<Environment>& _environments;
};

} // namespace clox
