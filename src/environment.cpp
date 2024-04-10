#include <unordered_map>
#include <format>
#include "environment.h"
#include "runtime_error.h"

namespace clox {

Value
Environment::get(Token token) {
  std::string name = token._lexeme;
  auto it = values.find(name);
  if (it == values.end())
    throw RuntimeError(token, std::format("Undefined variable: {}.", name));
  return it->second;
}

void
Environment::define(Token token, Value value) {
  std::string name = token._lexeme;
  values[name] = value;
}

void
Environment::assign(Token token, Value value) {
  std::string name = token._lexeme;
  auto it = values.find(name);
  if (it == values.end())
    throw RuntimeError(token, std::format("Undefined variable: {}.", name));
  it->second = value;
}


} // namespace clox
