#pragma once

#include <memory>
#include <vector>
#include <string>
#include "token.h"

namespace clox {

template <typename T>
class Expr {
public:
  virtual ~Expr() = default;
  virtual T
  accept(Visitor<T> &visitor) const = 0;
};

template <typename T>
class Visitor {
public:
    virtual T visit(const Binary &expr) = 0;
    virtual T visit(const Grouping &expr) = 0;
    virtual T visit(const Literal &expr) = 0;
    virtual T visit(const Unary &expr) = 0;
};


// Expression Binary
template <typename T>
class Binary : public Expr {
public:
  explicit Binary(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) : left(std::move(left)), op(std::move(op)), right(std::move(right))  {}

  T accept(Visitor<T> &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> left;
  Token op;
  std::unique_ptr<Expr> right;
};

// Expression Grouping
template <typename T>
class Grouping : public Expr {
public:
  explicit Grouping(std::unique_ptr<Expr> expression) : expression(std::move(expression))  {}

  T accept(Visitor<T> &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> expression;
};

// Expression Literal
template <typename T>
class Literal : public Expr {
public:
  explicit Literal(Value value) : value(std::move(value))  {}

  T accept(Visitor<T> &visitor) const override {
    return visitor.visit(*this);
  }

  Value value;
};

// Expression Unary
template <typename T>
class Unary : public Expr {
public:
  explicit Unary(Token op, std::unique_ptr<Expr> right) : op(std::move(op)), right(std::move(right))  {}

  T accept(Visitor<T> &visitor) const override {
    return visitor.visit(*this);
  }

  Token op;
  std::unique_ptr<Expr> right;
};

}
