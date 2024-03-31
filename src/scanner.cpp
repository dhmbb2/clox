#include <scanner.h>
#include <bits/stdc++.h>
#include <map>
#include "interpreter_driver/error_handler.h"

namespace loxcc
{


/// @brief look ahead one character
/// @param expected the expected character
/// @return if the next character meets expected
bool
Scanner::match(char expected) {
  if (isAtEnd()) return false;
  if (_source[_cur] != expected) return false;

  _cur++;
  return true;\
}

/// @brief look ahead one character (do not move the pointer)
/// @return return the next character
char
Scanner::peek() {
  if (isAtEnd()) return false;
  return _source[_cur];
}

/// @brief look ahead two characters (do not move the pointer)
/// @return return the next character
char
Scanner::peekNext() {
  if (_cur + 1 >= _source.length()) return '\0';
  return _source[_cur + 1];
}

/// @brief add a string to the token list
void
Scanner::string() {
  while(peek() != '"' && !isAtEnd()) {
    if (peek() == '\n') ++_line;
    advance();
  }

  if (isAtEnd()) {
    error(_line, "Unterminated string.");
    return;
  }

  advance(); // consume the closing "

  std::string value = _source.substr(_start + 1, _cur - 1);
  addToken(TokenType::STRING, Value{value});
}

/// @brief add a number to the token list
void
Scanner::number() {
  while (isDigit(peek())) advance();

  // Look for a fractional part.
  if (peek() == '.' && isDigit(peekNext())) {
    // Consume the "."
    advance();

    while (isDigit(peek())) advance();
  }

  addToken(TokenType::NUMBER, Value{std::stod(_source.substr(_start, _cur))});
}

/// @brief add an identifier to the token list
void
Scanner::identifier() {
  while(isAlphaDigit(peek()) && !isAtEnd()) 
    advance();

  // See if the identifier is a reserved word.
  std::string text = _source.substr(_start, _cur);
  auto it = _keywords.find(text);
  if (it != _keywords.end()) {
    addToken(TokenType::IDENTIFIER);
    return;
  }
  
  addToken(it->second);
}

void
Scanner::init_keyword_map() {
  _keywords["and"] = TokenType::AND;
  _keywords["class"] = TokenType::CLASS;
  _keywords["else"] = TokenType::ELSE;
  _keywords["false"] = TokenType::FALSE;
  _keywords["for"] = TokenType::FOR;
  _keywords["fun"] = TokenType::FUN;
  _keywords["if"] = TokenType::IF;
  _keywords["nil"] = TokenType::NIL;
  _keywords["or"] = TokenType::OR;
  _keywords["print"] = TokenType::PRINT;
  _keywords["return"] = TokenType::RETURN;
  _keywords["super"] = TokenType::SUPER;
  _keywords["this"] = TokenType::THIS;
  _keywords["true"] = TokenType::TRUE;
  _keywords["var"] = TokenType::VAR;
  _keywords["while"] = TokenType::WHILE;
}

void 
Scanner::scanTokens() {
  while(!isAtEnd()) {
    _start = _cur;
    scanToken();
 }
  // append an EOF at the end of the vector
  addToken(TokenType::MYEOF);
}

void 
Scanner::scanToken() {
  char c = advance();
  switch (c)
  {
  // Single character tokens
  case '(': addToken(TokenType::LEFT_PAREN); break;
  case ')': addToken(TokenType::RIGHT_PAREN); break;
  case '{': addToken(TokenType::LEFT_BRACE); break;
  case '}': addToken(TokenType::RIGHT_BRACE); break;
  case ',': addToken(TokenType::COMMA); break;
  case '.': addToken(TokenType::DOT); break;
  case '-': addToken(TokenType::MINUS); break;
  case '+': addToken(TokenType::PLUS); break;
  case ';': addToken(TokenType::SEMICOLON); break;
  case '*': addToken(TokenType::STAR); break;

  // One or two character tokens
  case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
  case '=': addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
  case '<': addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
  case '>': addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER); break;

  // longer lexemes
  case '/':
    if (match('/')) {
      // A comment goes until the end of the line. 
      // Do not add line number.
      while (peek() != '\n' && !isAtEnd()) advance();
    } else {
      addToken(TokenType::SLASH);
    }
    break;

  // ignore whitespaces
  case ' ':
  case '\r':
  case '\t':
    break;
  
  // meet string
  case '"': string(); break;


  default:
    if (std::isdigit(c))
      number();
    else if (isAlpha(c)) {
      identifier();
    }
    else
      error(_line, "meet unexpected character");
    break;
  }
}

} // namespace loxcc

