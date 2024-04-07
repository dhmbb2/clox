#pragma once
#include "Expr.h"
#include "parser.h"
#include "runtime_error.h"

namespace clox
{

class Interpreter: public Visitor {
public:
  Interpreter() = default;

  ReturnValType visit(const Binary &expr) override;
  ReturnValType visit(const Grouping &expr) override;
  ReturnValType visit(const Literal &expr) override;
  ReturnValType visit(const Unary &expr) override;

  void interpret(const Expr& expr);
  ReturnValType evaluate(const Expr& expr);
  
  // error reporting functions
  void checkNumber(const Token &op, const Value &operand) {
    if (Value::whichType(operand) == ValueType::NUMBER) {
      return;
    }
    throw RuntimeError(op, "Operand must be a number.");
  }
  void checkNumber(const Token &op, const Value &left, const Value &right) {
    if (Value::whichType(left) == ValueType::NUMBER && Value::whichType(right) == ValueType::NUMBER) {
      return;
    }
    throw RuntimeError(op, "Operands must be numbers.");
  }

};

} // namespace clox
