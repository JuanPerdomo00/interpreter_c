#include "parser/parser.h"
#include "parser/expr.h"
#include "parser/helpers.h"
#include "lexer/token.h"
#include "lexer/lexer.h"
#include <stdio.h>
#include <stdlib.h>



static Token next_valid_token(Parser *p) {
  TokenResult tr;

  while (1) {
    tr = lexer_next_token(p->lexer);

    if (tr.kind == TR_ERROR) {
      fprintf(stderr,
              "error: %s\n  --> line %d, column %d\n",
              tr.error.msg,
              tr.error.line,
              tr.error.column);
      continue;
    }

    return tr.token;
  }
}


Parser parser_create(Lexer *lexer) {
  Parser p;
  p.lexer = lexer;
  p.current = next_valid_token(&p);
  p.arena = arena_alloc_create(1024 * 1024);
  p.error_reported = false;
  return p;
}

void parser_destroy(Parser *p) {
  arena_free(p->arena);
}


bool parser_is_at_end(Parser *p) {
  return p->current.kind == TOKEN_EOF;
}

Token *parser_peek(Parser *p) {
  return &p->current;
}

void parser_advance(Parser *p) {
  p->current = next_valid_token(p);
}


void parser_error(Parser *p, const char *msg) {
  if(p->error_reported) return;
  p->error_reported = true;

  Token *t = parser_peek(p);

  fprintf(stderr, "error: %s\n", msg);
  fprintf(stderr, "  --> line %d, column %d\n",
          t->line, t->column);

  const char *source = p->lexer->buffer;
  const char *line_start = t->raw.ptr;

  while (line_start > source && *(line_start - 1) != '\n')
    line_start--;

  const char *line_end = t->raw.ptr;
  while (*line_end && *line_end != '\n')
    line_end++;

  fprintf(stderr, "  %.*s\n",
          (int)(line_end - line_start), line_start);
  fprintf(stderr, "  %*s^\n", t->column - 1, "");
}

void parser_sync(Parser *p) {
  while (!parser_is_at_end(p)) {
    switch (parser_peek(p)->kind) {

      case TOKEN_SEMICOLON:
      case TOKEN_RBRACE:
        parser_advance(p);
        p->error_reported = false;
        return;

      case TOKEN_IF:
      case TOKEN_WHILE:
      case TOKEN_RETURN:
      case TOKEN_FUNC:
        p->error_reported = false;
        return;

      default:
        parser_advance(p);
    }
  }

  p->error_reported = false;
}

