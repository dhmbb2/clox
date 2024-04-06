#pragma once

#include <string>
#include <optional>
#include <iostream>

namespace clox {

enum class TokenType {
  // Single-character tokens.
  LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
  COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

  // One or two character tokens.
  BANG, BANG_EQUAL,
  EQUAL, EQUAL_EQUAL,
  GREATER, GREATER_EQUAL,
  LESS, LESS_EQUAL,

  // Literals.
  IDENTIFIER, STRING, NUMBER,

  // Keywords.
  AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR,
  PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE,

  MYEOF
};

std::string 
type2string(TokenType);

class Value {
public:
  Value(): _s(std::nullopt), _d(std::nullopt) {};
  Value(std::string s): _s({s}), _d(std::nullopt) {}
  Value(double d): _s(std::nullopt), _d({d}) {} 

  std::optional<std::string> _s;
  std::optional<double> _d;

  std::string
  toString() const {
    if (_s.has_value()) {
      return _s.value();
    } else {
      return std::to_string(_d.value());
    }
  }
};

class Token {
public:
  Token(TokenType type, std::optional<std::string> lexeme, std::optional<Value> literal, int line): 
    _type(type), _lexeme(lexeme), _literal(literal), _line(line) {}

  static std::string
  toString(Token token) {
    std::string lexeme = token._lexeme.value_or("");
    return type2string(token._type) + " " + lexeme + " " +std::to_string(token._line);
  }

  TokenType _type;
  std::optional<std::string> _lexeme;
  std::optional<Value> _literal;
  int _line;
};

}