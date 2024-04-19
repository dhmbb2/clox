#pragma once

#include "scanner.h"
#include "Expr.h"
#include "Stmt.h"
#include "interpreter_driver/error_handler.h"
#include <vector>

namespace clox
{

/// @brief Parser, adopting Recursice descent parsing
class Parser {
public:
  Parser(Scanner scanner): _scanner(scanner) {
    _tokens = _scanner._tokens;
  }

  class ParserError : public std::exception {};

  // helper functions for traversing token lists
  template<typename... Args>
  bool match(Args... args) {
    return (... || (check(args) ? advance(), true : false));
  }

  bool
  check(TokenType type) {
    if (isAtEnd()) return false;
    return peek()._type == type;
  }

  Token
  advance() {
    if (!isAtEnd()) _current++;
    return previous();
  }

  Token
  previous() {
    return _tokens[_current - 1];
  }

  bool
  isAtEnd() {
    return peek()._type == TokenType::MYEOF;
  }

  Token
  peek() {
    return _tokens[_current];
  }

  ParserError
  parser_error(Token token, std::string message) {
    error(token, message);
    return ParserError();
  }

  Token
  consume(TokenType type, std::string message) {
    if (check(type)) return advance();
    throw parser_error(peek(), message);
  }

  std::vector<std::unique_ptr<Stmt>> parse();

  // funtions that implement the grammar
  std::unique_ptr<Expr> expression();
  std::unique_ptr<Expr> assignment();
  std::unique_ptr<Expr> equality();
  std::unique_ptr<Expr> comparison();
  std::unique_ptr<Expr> term();
  std::unique_ptr<Expr> factor();
  std::unique_ptr<Expr> unary();
  std::unique_ptr<Expr> primary();
  std::unique_ptr<Expr> variable();

  std::unique_ptr<Stmt> declaration();
  std::unique_ptr<Stmt> Vardeclaration();
  std::unique_ptr<Stmt> statement();
  std::unique_ptr<Stmt> printStatement();
  std::unique_ptr<Stmt> expressionStatement();
  std::unique_ptr<Stmt> blockStatement();
  std::unique_ptr<Stmt> ifStatement();
  std::unique_ptr<Stmt> whileStatement();
  std::unique_ptr<Stmt> forStatement();

  // syn for error
  void synchronize();

  Scanner _scanner;
  std::vector<Token> _tokens;
  int _current = 0;

};

} // namespace clox
