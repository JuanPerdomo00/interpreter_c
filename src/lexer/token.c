#include "lexer/token.h"
#include "lexer/kind.h"
#include <stdio.h>

Token token_create(TokenKind kind, StrView raw,
                   uint32_t line, uint32_t column) {
  return (Token){.kind = kind,
                 .raw = raw,
                 .line = line,
                 .column = column};
}

void debug_token(Token *t) {
  printf("Token(kind=%s,data=\"%.*s\", length=%d, line=%d, column=%d)\n",
         kind_debug(t->kind),
         t->raw.len, t->raw.ptr,
         t->raw.len,
         t->line,
         t->column);
}
