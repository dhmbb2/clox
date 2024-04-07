#include "interpreter.h"
#include <utility>
#include "value.h"

namespace clox
{
    
ReturnValType
Interpreter::visit(const Binary &expr) {
  Value left = std::get<Value>(evaluate(*expr.left.get()));
  Value right = std::get<Value>(evaluate(*expr.right.get()));
  switch (expr.op._type)
  {
  case TokenType::MINUS:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() - right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::PLUS:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() + right.number())};
    } 
    if (Value::whichType(left) == ValueType::STRING && Value::whichType(right) == ValueType::STRING) {
        return {Value(left.string() + right.string())};
    }
    throw RuntimeError(expr.op, "Operands must be two numbers or two strings.");
    break;
  case TokenType::SLASH:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() / right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::STAR:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() * right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::GREATER:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() > right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::GREATER_EQUAL:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() >= right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::LESS:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() < right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::LESS_EQUAL:
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
        return {Value(left.number() <= right.number())};
    }
    checkNumber(expr.op, left, right);
    break;
  case TokenType::BANG_EQUAL:
    return {Value(left._value != right._value)};
  case TokenType::EQUAL_EQUAL:
    return {Value(left._value == right._value)};

  default:
      break;
  }
}

ReturnValType
Interpreter::visit(const Grouping &expr) {
  return evaluate(*expr.expression.get());
}

ReturnValType
Interpreter::visit(const Literal &expr) {
  return {expr.value};
}

ReturnValType
Interpreter::visit(const Unary &expr) {
  Value right = std::get<Value>(evaluate(*expr.right.get()));
  switch (expr.op._type) {
    case TokenType::MINUS:
      if (Value::whichType(right) == ValueType::NUMBER) {
          return {Value(-right.number())};
      }
      checkNumber(expr.op, right);
      break;
    case TokenType::BANG:
      return {Value(!Value::isTruthy(right))};
      break;
    default:
      break;
  }
  return {};
}

ReturnValType
Interpreter::evaluate(const Expr& expr) {
  return expr.accept(*this);
}

void
Interpreter::interpret(const Expr& expr) {
  try {
    Value result = std::get<Value>(evaluate(expr));
    std::cout << Value::toString(result) << std::endl;
  } catch (const RuntimeError &error) {
    runtime_error(error);
  }
}


} // namespace clox
