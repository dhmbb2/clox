#pragma once

#include <memory>
#include <vector>
#include <string>
#include <variant>
#include "token.h"
#include "Expr.h"

namespace clox {

using ReturnValType = std::variant<std::monostate, Value, std::string, size_t>;

class Visitor;
class Stmt;
class Expression;
class Print;
class Var;

class Stmt {
public:
class Visitor {
public:
    virtual ReturnValType visit(const Expression &stmt) = 0;
    virtual ReturnValType visit(const Print &stmt) = 0;
    virtual ReturnValType visit(const Var &stmt) = 0;
};

virtual ~Stmt() = default;
  virtual ReturnValType
  accept(Visitor &visitor) const = 0;
};

// Expression Expression
class Expression : public Stmt {
public:
  explicit Expression(std::unique_ptr<Expr> expression) : expression(std::move(expression))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> expression;
};

// Expression Print
class Print : public Stmt {
public:
  explicit Print(std::unique_ptr<Expr> expression) : expression(std::move(expression))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> expression;
};

// Expression Var
class Var : public Stmt {
public:
  explicit Var(Token name, std::unique_ptr<Expr> initializer) : name(std::move(name)), initializer(std::move(initializer))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token name;
  std::unique_ptr<Expr> initializer;
};

}
