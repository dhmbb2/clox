#pragma once

#include <string>
#include <optional>
#include <iostream>
#include <variant>
#include "value.h"

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