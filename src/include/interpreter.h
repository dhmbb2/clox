#pragma once
#include <vector>
#include "Expr.h"
#include "Stmt.h"
#include "parser.h"
#include "runtime_error.h"
#include "environment.h"

namespace clox
{

class Interpreter: public Expr::Visitor, public Stmt::Visitor {
public:
  Interpreter() {
    global = std::make_shared<Environment>();
    environment = global;
  }

  ReturnValType visit(const Binary &expr) override;
  ReturnValType visit(const Grouping &expr) override;
  ReturnValType visit(const Literal &expr) override;
  ReturnValType visit(const Unary &expr) override;
  ReturnValType visit(const Variable &expr) override;
  ReturnValType visit(const Assignment &expr) override;
  ReturnValType visit(const Expression &stmt) override;
  ReturnValType visit(const Print &stmt) override;
  ReturnValType visit(const Var &stmt) override;
  ReturnValType visit(const Block &stmt) override;
  ReturnValType visit(const IF &stmt) override;
  ReturnValType visit(const WHILE &stmt) override;

  void interpret(std::vector<std::unique_ptr<Stmt>> stmts);
  ReturnValType evaluate(const Expr& expr);
  ReturnValType execute(const Stmt& stmt);
  
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

  std::shared_ptr<Environment> global;
  std::shared_ptr<Environment> environment;
};

} // namespace clox
