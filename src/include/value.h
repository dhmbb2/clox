#pragma once
#include <string>
#include <variant>

namespace clox {

enum class ValueType {
  STRING, NUMBER, BOOL, NIL
};

/// @brief Lox's value type system, which is a variant of string, double, and int8_t.
// number: double
// string: std::string
// bool: bool
// nil std::monostate
class Value {
public:
  Value() {}
  Value(std::string value): _value(value) {}
  Value(double value): _value(value) {}
  Value(bool value): _value(value) {}

  static ValueType
  whichType(Value value) {
    if (std::holds_alternative<std::string>(value._value)) return ValueType::STRING;
    if (std::holds_alternative<double>(value._value)) return ValueType::NUMBER;
    if (std::holds_alternative<bool>(value._value)) return ValueType::BOOL;
    return ValueType::NIL;
  }

  static std::string
  toString(Value value) {
    if (std::holds_alternative<std::string>(value._value)) 
      return std::get<std::string>(value._value);
    if (std::holds_alternative<double>(value._value)) 
      return std::to_string(std::get<double>(value._value));
    if (std::holds_alternative<bool>(value._value)) 
      return std::get<bool>(value._value) ? "true" : "false";
    return "nil";
  }

  double
  number() const {
    if (std::holds_alternative<double>(_value)) 
      return std::get<double>(_value);
    return 0;
  }

  std::string
  string() const {
    if (std::holds_alternative<std::string>(_value)) 
      return std::get<std::string>(_value);
    return "";
  }

  bool
  boolean() const {
    if (std::holds_alternative<bool>(_value)) 
      return std::get<bool>(_value);
    return false;
  }

  static bool
  isTruthy(const Value &value) {
    if (Value::whichType(value) == ValueType::NIL)
      return false;
    if (Value::whichType(value) == ValueType::BOOL) {
      return value.boolean();
    }
    return true;
  }

  std::variant<std::monostate, std::string, double, bool> _value;
};

}