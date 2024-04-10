#include "parser.h"
#include "token.h"
#include <vector>
#include <memory>
#include <utility>


namespace clox {  

  std::vector<std::unique_ptr<Stmt>>
  Parser::parse() {
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd())
      statements.emplace_back(declaration());
    return std::move(statements);
  }

  std::unique_ptr<Expr>
  Parser::expression() {
    return std::move(assignment());
  }

  std::unique_ptr<Expr>
  Parser::assignment() {
    auto left = equality();

    if (!match(TokenType::EQUAL))
      return std::move(left);

    Token equal = previous();
    // if the left is an variable
    if (auto var = dynamic_cast<Variable*>(left.get()); var != nullptr) {
      Token token = var->name;
      auto value = assignment();
      return std::make_unique<Assignment>(token, std::move(value));
    }

    // this is because we can transpass this mistake by returning it as a  
    // equality() expression, we don't need to panic
    parser_error(equal, "Invalid assignment target.");
    return std::move(left);
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
    if (match(TokenType::IDENTIFIER))
      return std::move(variable());

    if (match(TokenType::LEFT_PAREN)) {
      auto expr = expression();
      consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
      return std::make_unique<Grouping>(std::move(expr));
    }

    throw parser_error(peek(), "Expect expression.");
    
  }


  std::unique_ptr<Expr>
  Parser::variable() {
    return std::make_unique<Variable>(previous());
  }

  std::unique_ptr<Stmt>
  Parser::declaration() {
    try {
      if (match(TokenType::VAR)) return Vardeclaration();
      return statement();
    } catch (ParserError) {
      synchronize();
      return nullptr;
    }
  }

  std::unique_ptr<Stmt>
  Parser::Vardeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expect variable name.");

    // if an assignment is present
    if (match(TokenType::EQUAL)) {
      auto initializer = expression();
      consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
      return std::make_unique<Var>(name, std::move(initializer));
    }

    consume(TokenType::SEMICOLON, "Expect ';' after variable declaration.");
    return std::make_unique<Var>(name, nullptr);
  }

  std::unique_ptr<Stmt>
  Parser::statement() {
    if (match(TokenType::PRINT))
      return std::move(printStatement());
    
    return expressionStatement();
  }

  std::unique_ptr<Stmt>
  Parser::printStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after statement.");
    return std::make_unique<Print>(std::move(expr));
  }

  std::unique_ptr<Stmt>
  Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::SEMICOLON, "Expect ';' after statement.");
    return std::make_unique<Expression>(std::move(expr));
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