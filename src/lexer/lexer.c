#include "lexer/lexer.h"
#include "lexer/kind.h"
#include "lexer/token.h"
#include "lexer/utils.h"
#include "utils/str_view.h"
#include "utils/vector.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>


static inline bool is_digit(char c) {
  return c >= '0' && c <= '9';
}

static inline bool is_ident_start(char c) {
  return (c >= 'a' && c <= 'z') ||
         (c >= 'A' && c <= 'Z') ||
         c == '_';
}

static inline bool is_ident_continue(char c) {
  return is_ident_start(c) || is_digit(c);
}

static inline StrView lexer_str_view(Lexer *l) {
  return (StrView){
      .ptr = l->buffer + l->token_start,
      .len = l->pos - l->token_start
  };
}


Lexer lexer_create(const char *filename, FILE *file) {
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  ArenaAllocator *arena = arena_alloc_create(size + 1);
  char *buffer = arena_push(arena, NULL, size + 1, alignof(char));
  fread(buffer, 1, size, file);
  buffer[size] = '\0';

  return (Lexer){
      .filename = filename,
      .buffer = buffer,
      .arena = arena,
      .length = size + 1,
      .pos = 0,
      .line = 1,
      .column = 1
  };
}

void lexer_destroy(Lexer *l) {
  arena_free(l->arena);
}


char lexer_peek(Lexer *l) {
  return l->buffer[l->pos];
}

char lexer_advance(Lexer *l) {
  char c = l->buffer[l->pos++];

  if (c == '\n') {
    l->line++;
    // l->column = 1;
  } 

  return c;
}

bool lexer_is_at_end(Lexer *l) {
  return l->pos >= l->length - 1;
}


static inline bool is_trash(char c) {
  return c == ' ' || c == '\n' || c == '\t' || c == '\r';
}

void lexer_trash(Lexer *l) {
  while (!lexer_is_at_end(l) && is_trash(lexer_peek(l))) {
    lexer_advance(l);
  }
}


static inline TokenResult tr_token(Token t) {
  return (TokenResult){ .kind = TR_TOKEN, .token = t };
}

static inline TokenResult tr_error(LexerError e) {
  return (TokenResult){ .kind = TR_ERROR, .error = e };
}

Token token_make(Lexer *l, TokenKind kind) {
  return token_create(kind, lexer_str_view(l),
                      l->token_line, l->token_column);
}

/* ===================================================== */
/* Lexers                                                */
/* ===================================================== */

TokenResult lexer_lex_identifier(Lexer *l) {
  lexer_advance(l); // consume first letter

  while (!lexer_is_at_end(l) && is_ident_continue(lexer_peek(l))) {
    lexer_advance(l);
  }

  return tr_token(token_make(l, TOKEN_IDENTIFIER));
}

TokenResult lexer_lex_number(Lexer *l) {
  while (!lexer_is_at_end(l) && is_digit(lexer_peek(l))) {
    lexer_advance(l);
  }

  return tr_token(token_make(l, TOKEN_INT_LITERAL));
}

// operators

TokenResult lexer_lex_add(Lexer *l) {
  lexer_advance(l); // consume '+'

  if (!lexer_is_at_end(l) && lexer_peek(l) == '+') {
    lexer_advance(l);
    return tr_token(token_make(l, TOKEN_PLUS_PLUS));
  }

  return tr_token(token_make(l, TOKEN_PLUS));
}

TokenResult lexer_lex_minus(Lexer *l) {
  lexer_advance(l);

  if (!lexer_is_at_end(l) && lexer_peek(l) == '-') {
    lexer_advance(l);
    return tr_token(token_make(l, TOKEN_MINUS_MINUS));
  }

  return tr_token(token_make(l, TOKEN_MINUS));
}


TokenResult lexer_lex_mul(Lexer *l) {
  lexer_advance(l);

  if (!lexer_is_at_end(l) && lexer_peek(l) == '*') {
    lexer_advance(l);
    return tr_token(token_make(l, TOKEN_STAR_STAR));
  }

  return tr_token(token_make(l, TOKEN_STAR));
}


TokenResult lexer_lex_div(Lexer *l) {
  lexer_advance(l);

  if (!lexer_is_at_end(l) && lexer_peek(l) == '/') {
    lexer_advance(l);
    return tr_token(token_make(l, TOKEN_SLASH_SLASH));
  }

  return tr_token(token_make(l, TOKEN_SLASH));
}



// dispatcher
TokenResult lexer_next_token(Lexer *l) {
  lexer_trash(l);

  l->token_start = l->pos;
  l->token_line = l->line;
  l->token_column = l->column;

  if (lexer_is_at_end(l)) {
    l->token_start = l->pos;
    return tr_token(token_make(l, TOKEN_EOF));
  }

  char c = lexer_peek(l);

  if (is_digit(c))        return lexer_lex_number(l);
  if (is_ident_start(c)) return lexer_lex_identifier(l);

  switch (c) {
    case '+': return lexer_lex_add(l);
    case '-': return lexer_lex_minus(l);
    case '*': return lexer_lex_mul(l);
    case '/': return lexer_lex_div(l);

    case '(':
      lexer_advance(l);
      return tr_token(token_make(l, TOKEN_LPAREN));

    case ')':
      lexer_advance(l);
      return tr_token(token_make(l, TOKEN_RPAREN));

    case ';':
      lexer_advance(l);
      return tr_token(token_make(l, TOKEN_SEMICOLON));

    default:
      return tr_error((LexerError){
          .msg = "unknown character",
          .line = l->line,
          .column = l->column
      });
  }
}


// tokenizer
ResultLexer lexer_tokenize(Lexer *l) {
  ResultLexer r;
  r.tokens = vector_create(sizeof(Token),5);
  r.errors = vector_create(sizeof(LexerError),5);

  while (true) {
    TokenResult tr = lexer_next_token(l);

    if (tr.kind == TR_ERROR) {
      vector_push(&r.errors, &tr.error);
      continue;
    }

    if (tr.token.kind == TOKEN_EOF)
      break;

    vector_push(&r.tokens, &tr.token);
  }

  return r;
}

