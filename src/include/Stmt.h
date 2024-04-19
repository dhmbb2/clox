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
class Block;
class IF;
class WHILE;

class Stmt {
public:
class Visitor {
public:
    virtual ReturnValType visit(const Expression &stmt) = 0;
    virtual ReturnValType visit(const Print &stmt) = 0;
    virtual ReturnValType visit(const Var &stmt) = 0;
    virtual ReturnValType visit(const Block &stmt) = 0;
    virtual ReturnValType visit(const IF &stmt) = 0;
    virtual ReturnValType visit(const WHILE &stmt) = 0;
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

// Expression Block
class Block : public Stmt {
public:
  explicit Block(std::vector<std::unique_ptr<Stmt>> statements) : statements(std::move(statements))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  std::vector<std::unique_ptr<Stmt>> statements;
};

// Expression IF
class IF : public Stmt {
public:
  explicit IF(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch) : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> thenBranch;
  std::unique_ptr<Stmt> elseBranch;
};

// Expression WHILE
class WHILE : public Stmt {
public:
  explicit WHILE(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) : condition(std::move(condition)), body(std::move(body))  {}

  ReturnValType accept(Visitor &visitor) const override {
    return visitor.visit(*this);
  }

  std::unique_ptr<Expr> condition;
  std::unique_ptr<Stmt> body;
};

}
