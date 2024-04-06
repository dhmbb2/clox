#include "parser.h"
#include "token.h"
#include <vector>
#include <memory>
#include <utility>


namespace clox {  

  std::unique_ptr<Expr>
  Parser::parser() {
    try {
      return std::move(expression());
    } catch (ParserError) {
      return nullptr;
    }
  }

  std::unique_ptr<Expr>
  Parser::expression() {
    return std::move(equality());
  }

  std::unique_ptr<Expr>
  Parser::equality() {
    auto left = comparison();

    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
      Token op = previous();
      auto right = comparison();
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return std::move(left);
  }

  std::unique_ptr<Expr>
  Parser::comparison() {
    auto left = term();

    while (match(TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL)) {
      Token op = previous();
      auto right = term();
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }

    return std::move(left);
  }

  std::unique_ptr<Expr>
  Parser::term() {
    auto left = factor();
    
    while (match(TokenType::MINUS, TokenType::PLUS)) {
      Token op = previous();
      auto right = factor(); 
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return std::move(left);
  }

  std::unique_ptr<Expr>
  Parser::factor() {
    auto left = unary();
    
    while (match(TokenType::SLASH, TokenType::STAR)) {
      Token op = previous();
      auto right = unary();
      left = std::make_unique<Binary>(std::move(left), op, std::move(right));
    }
    return std::move(left);
  }

  std::unique_ptr<Expr>
  Parser::unary() {
    if (match(TokenType::BANG, TokenType::MINUS)) {
      Token op = previous();
      auto right = unary();
      return std::make_unique<Unary>(op, std::move(right));
    }
    return std::move(primary());
  }

  std::unique_ptr<Expr>
  Parser::primary() {
    if (match(TokenType::FALSE)) return std::make_unique<Literal>(Value(false));
    if (match(TokenType::TRUE)) return std::make_unique<Literal>(Value(true));
    if (match(TokenType::NIL)) return std::make_unique<Literal>(Value());
    if (match(TokenType::NUMBER, TokenType::STRING)) 
      return std::make_unique<Literal>(previous()._literal.value());

    if (match(TokenType::LEFT_PAREN)) {
      auto expr = expression();
      consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
      return std::make_unique<Grouping>(std::move(expr));
    }

    throw parser_error(peek(), "Expect expression.");
    
  }

  void
  Parser::synchronize(){
    advance();

    while (!isAtEnd()) {
      if (previous()._type == TokenType::SEMICOLON) return;

      switch (peek()._type) {
        case TokenType::CLASS:
        case TokenType::FUN:
        case TokenType::VAR:
        case TokenType::FOR:
        case TokenType::IF:
        case TokenType::WHILE:
        case TokenType::PRINT:
        case TokenType::RETURN:
          return;
        default:
          advance();
      }
    }
  }

}