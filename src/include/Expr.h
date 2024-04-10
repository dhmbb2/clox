#pragma once

#include <memory>
#include <vector>
#include <string>
#include <variant>
#include "token.h"

namespace clox {

using ReturnValType = std::variant<std::monostate, Value, std::string, size_t>;

class Visitor;
class Expr;
class Binary;
class Grouping;
class Literal;
class Unary;
class Assignment;
class Variable;

class Expr {
public:
class Visitor {
public:
    virtual ReturnValType visit(const Binary &expr) = 0;
    virtual ReturnValType visit(const Grouping &expr) = 0;
    virtual ReturnValType visit(const Literal &expr) = 0;
    virtual ReturnValType visit(const Unary &expr) = 0;
    virtual ReturnValType visit(const Assignment &expr) = 0;
    virtual ReturnValType visit(const Variable &expr) = 0;
};

virtual ~Expr() = default;
  virtual ReturnValType
  accept(Visitor &visitor) const = 0;
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

// Expression Assignment
class Assignment : public Expr {
public:
  explicit Assignment(Token name, std::unique_ptr<Expr> value) : name(std::move(name)), value(std::move(value))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token name;
  std::unique_ptr<Expr> value;
};

// Expression Variable
class Variable : public Expr {
public:
  explicit Variable(Token name) : name(std::move(name))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  Token name;
};

}
