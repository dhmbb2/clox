#pragma once

#include <unordered_map>
#include "value.h"
#include "token.h"

namespace clox {

class Environment {
public:

  Value get(Token);
  void define(Token, Value);
  void assign(Token, Value);


  std::unordered_map<std::string, Value> values;
};

} // namespace clox
