#ifndef TOKEN_H
#define TOKEN_H
#include "lexer/kind.h"
#include "utils/str_view.h"
#include <stdint.h>

typedef struct Token Token;

struct Token {
  bool owns;
  TokenKind kind;
  StrView raw;
  uint32_t line;
  uint32_t column;
};

Token token_create(TokenKind kind, StrView raw, uint32_t line, uint32_t column);

void debug_token(Token *t);

void token_string_destroy(Token* t);

#endif
