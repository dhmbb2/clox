#pragma once

#include <memory>
#include <vector>
#include <string>
#include <variant>
#include "token.h"

namespace clox {

using ReturnValType = std::variant<double, std::string, bool>;

class Visitor;
class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;

class Expr {
public:
  virtual ~Expr() = default;
  virtual ReturnValType
  accept(Visitor &visitor) const = 0;
};

class Visitor {
public:
    virtual ReturnValType visit(const Binary &expr) = 0;
    virtual ReturnValType visit(const Grouping &expr) = 0;
    virtual ReturnValType visit(const Literal &expr) = 0;
    virtual ReturnValType visit(const Unary &expr) = 0;
};


// Expression Binary
class Binary : public Expr {
public:
  explicit Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) : left(std::move(left)), op(std::move(op)), right(std::move(right))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> left;
  Token op;
  std::unique_ptr<Expr> right;
};

// Expression Grouping
class Grouping : public Expr {
public:
  explicit Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> expression;
};

// Expression Literal
class Literal : public Expr {
public:
  explicit Literal(Value value) : value(std::move(value))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  Value value;
};

// Expression Unary
class Unary : public Expr {
public:
  explicit Unary(Token op, std::unique_ptr<Expr> right) : op(std::move(op)), right(std::move(right))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token op;
  std::unique_ptr<Expr> right;
};

}
