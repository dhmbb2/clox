#pragma once

#include <string>

namespace loxcc {

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
  Token(TokenType type, std::string lexeme, int line): 
    _type(type), _lexeme(lexeme), _line(line) {}

  std::string
  to_string() {
    return type2string(_type) + _lexeme + std::to_string(_line);
  }

  TokenType _type;
  std::string _lexeme;
  int _line;
};

}