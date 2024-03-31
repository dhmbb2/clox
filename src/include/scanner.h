#include <string>
#include <vector>
#include <utility>
#include <map>
#include <token.h>

namespace loxcc {

/// @brief Scanner class, with source code stored in _source
class Scanner {
public:
  Scanner(std::string source): _source(std::move(source)) {
    _start = _cur = 0;
    _line = 1;
    init_keyword_map();
  }

  void scanTokens();
  void scanToken();
  bool match(char expected);
  char peek();
  char peekNext();
  void string();
  void number();
  void identifier();
  void init_keyword_map();

  /// @brief move current pointer forward for one character
  /// @return a character in the source code
  char
  advance() {
    return _source[_cur++]; 
  }

  bool
  isAtEnd() {
    return _cur == _source.length();
  }

  bool
  isDigit(char c) {
    return (c >= '0' and c <='9');
  }

  bool 
  isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool 
  isAlphaDigit(char c) {
    return isAlpha(c) || isdigit(c);
  }

  void
  addToken(TokenType type) {
    _tokens.push_back(Token{type, std::nullopt, std::nullopt, _line});
  }

  void
  addToken(
    TokenType type,
    Value literal
  ) {
    std::string lexeme = _source.substr(_start, _cur);
    _tokens.push_back(Token{type, {lexeme}, {literal}, _line});
  }

  std::string _source;
  std::vector<Token> _tokens;
  std::map<std::string, TokenType> _keywords;
  int _start;
  int _line;
  int _cur;
};

}