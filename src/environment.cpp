#include <unordered_map>
#include <format>
#include "environment.h"
#include "runtime_error.h"

namespace clox {

Value
Environment::get(Token token) {
  std::string name = token._lexeme;
  
  auto it = _values.find(name);
  if (it != _values.end())
    return it->second;
  
  if (_enclosing != nullptr)
    return _enclosing->get(token);
  
  throw RuntimeError(token, std::format("Undefined variable: {}.", name));
}

void
Environment::define(Token token, Value value) {
  std::string name = token._lexeme;
  _values[name] = value;
}

void
Environment::assign(Token token, Value value) {
  std::string name = token._lexeme;
  auto it = _values.find(name);

  if (it != _values.end()) {
    it->second = value;
    return;
  }

  if (_enclosing != nullptr) {
    _enclosing->assign(token, value);
    return;
  }

  throw RuntimeError(token, std::format("Undefined variable: {}.", name));
}


} // namespace clox
