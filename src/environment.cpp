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

Value
EnvironmentStack::get(Token token) {
  std::string name = token._lexeme;
  for (int i = _environments.size()-1 ; i >= 0; --i) {
    try {
      Value v = _environments[i].get(token);
      return v;
    } catch(RuntimeError& error) {
      continue;
    }
  }
  throw RuntimeError(token, std::format("Undefined variable: {}.", name));
  return {};
}

void
EnvironmentStack::define(Token token, Value value) {
  _environments.back().define(token, value);
}

void
EnvironmentStack::assign(Token token, Value value) {
  std::string name = token._lexeme;
  for (int i = _environments.size()-1 ; i >= 0; --i) {
    try {
      _environments[i].assign(token, value);
    } catch(RuntimeError& error) {
      continue;
    }
  }
  throw RuntimeError(token, std::format("Undefined variable: {}.", name));
}

} // namespace clox
