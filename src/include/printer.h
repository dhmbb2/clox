#pragma once
#include "Expr.h"
#include <string>
#include <sstream>

namespace clox
{

class Printer: public Visitor {
public:
  std::string print(Expr* expr) {
    return std::get<std::string>(expr->accept(*this));
  }

  template<typename... Args>
  ReturnValType parenthesize(std::string opname, Args... exprs) {
    std::stringstream result;
    result << "(" << opname;
    ((result << " " << std::get<std::string>(exprs->accept(*this))), ...);
    return {result.str() + ")"};
  }

  ReturnValType visit(const Binary &expr) override {
    return parenthesize(expr.op._lexeme.value(), expr.left.get(), expr.right.get());
  }

  ReturnValType visit(const Grouping &expr) override {
    return parenthesize("group", expr.expression.get());
  }

  ReturnValType visit(const Literal &expr) override {
    return {expr.value.toString()};
  }

  ReturnValType visit(const Unary &expr) override {
    return parenthesize(expr.op._lexeme.value(), expr.right.get());
    }

};

} // namespace clox
