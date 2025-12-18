#include "lexer/lexer.h"
#include "lexer/kind.h"
#include "lexer/token.h"
#include "lexer/utils.h"
#include "utils/hashmap.h"
#include "utils/str_view.h"
#include "utils/vector.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

static StrView lexer_str_view(Lexer *l) {
  return (StrView){.len = l->pos - l->token_start,
                   .ptr = l->buffer + l->token_start};
}

static Hashmap keywords;
static bool keywords_initialized = false;

static StrView key_for = {.ptr = "for", .len = 3};
static StrView key_func = {.ptr = "func", .len = 4};
static StrView key_if = {.ptr = "if", .len = 2};
static StrView key_else = {.ptr = "else", .len = 4};
static StrView key_while = {.ptr = "while", .len = 5};
static StrView key_return = {.ptr = "return", .len = 6};

static void init_keywords_if_needed(void) {
  if (keywords_initialized)
    return;

  keywords = hashmap_create(strview_hash, strview_cmp);
  hashmap_insert(&keywords, &key_for, &(TokenKind){TOKEN_FOR},
                 sizeof(TokenKind));

  hashmap_insert(&keywords, &key_func, &(TokenKind){TOKEN_FUNC},
                 sizeof(TokenKind));

  hashmap_insert(&keywords, &key_if, &(TokenKind){TOKEN_IF}, sizeof(TokenKind));

  hashmap_insert(&keywords, &key_else, &(TokenKind){TOKEN_ELSE},
                 sizeof(TokenKind));

  hashmap_insert(&keywords, &key_while, &(TokenKind){TOKEN_WHILE},
                 sizeof(TokenKind));
  hashmap_insert(&keywords, &key_return, &(TokenKind){TOKEN_RETURN},
                 sizeof(TokenKind));

  keywords_initialized = true;
}

Lexer lexer_create(FILE *file) {
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  char *buffer = malloc(size + 1);
  fread(buffer, 1, size, file);

  return (Lexer){
      .buffer = buffer, .length = size, .pos = 0, .column = 1, .line = 1};
}

void result_lexer_destroy(ResultLexer *rl) {
  vector_destroy(&rl->errors);
  vector_destroy(&rl->tokens);
}

LexerError lexer_error_create(LexerErrorKind kind, Lexer *l, const char *msg) {
  return (LexerError){.msg = msg,
                      .start = l->token_start,
                      .length = l->pos - l->token_start,
                      .line = l->token_line,
                      .column = l->token_column};
}

char lexer_advance(Lexer *l) {
  char c = l->buffer[l->pos++];

  if (c == ' ') {
    l->column++;
  } else if (c == '\n') {
    l->line++;
    l->column = 1;
  }

  return c;
}

char lexer_peek(Lexer *l) { return l->buffer[l->pos]; }

bool lexer_is_at_end(Lexer *l) {
  if (l->pos >= l->length - 1) // -1 cause of \0
    return true;
  return false;
}

static inline TokenResult tr_token(Token t) {
  return (TokenResult){.kind = TR_TOKEN, .token = t};
}

static inline TokenResult tr_error(LexerError error) {
  return (TokenResult){.kind = TR_ERROR, .error = error};
}

Token token_make(Lexer *l, TokenKind k) {
  return token_create(k, lexer_str_view(l), l->token_line, l->token_column);
}

TokenResult lexer_lex_identifier(Lexer *l) {
  while (!lexer_is_at_end(l) && is_identifier(lexer_peek(l))) {
    lexer_advance(l);
  }
  return tr_token(token_make(l, TOKEN_IDENTIFIER));
}

TokenResult lexer_lex_number(Lexer *l) {
  bool seen_dot = false;

  while (!lexer_is_at_end(l)) {
    char c = lexer_peek(l);

    if (isdigit(c))
      lexer_advance(l);
    else if (c == '.') {
      if (seen_dot) {
        return tr_error(lexer_error_create(
            SYNTAX_ERROR, l, "invalid syntax in number try  \\d+.d*"));
      }
      seen_dot = true;
      lexer_advance(l);
    } else
      break;
  }
  return tr_token(
      token_make(l, seen_dot ? TOKEN_FLOAT_LITERAL : TOKEN_INT_LITERAL));
}

bool is_trash(char c) {
  if (c == ' ' || c == '\n')
    return true;
  return false;
}
void lexer_trash(Lexer *l) {
  while (!lexer_is_at_end(l) && is_trash(lexer_peek(l))) {
    lexer_advance(l);
  }
}

TokenResult lexer_lex_add(Lexer *l) {
  char c = lexer_peek(l);
  Token t = token_make(l, TOKEN_PLUS);

  if (c == '+') {
    lexer_advance(l);
    t = token_make(l, TOKEN_PLUS_PLUS);
  }

  return tr_token(t);
}

TokenResult lexer_lex_minus(Lexer *l) {
  char c = lexer_peek(l);
  Token t = token_make(l, TOKEN_MINUS);

  if (c == '-') {
    lexer_advance(l);
    t = token_make(l, TOKEN_MINUS_MINUS);
  }

  return tr_token(t);
}

TokenResult lexer_lex_equal(Lexer *l) {
  char c = lexer_peek(l);
  Token t = token_make(l, TOKEN_ASSIGN);

  if (c == '=') {
    lexer_advance(l);
    t = token_make(l, TOKEN_EQUAL);
  }

  return tr_token(t);
}

TokenResult lexer_lex_div(Lexer *l) {
  char c = lexer_peek(l);
  Token t = token_make(l, TOKEN_SLASH);

  if (c == '/') {
    lexer_advance(l);
    t = token_make(l, TOKEN_SLASH_SLASH);
  }

  return tr_token(t);
}

TokenResult lexer_lex_mul(Lexer *l) {
  char c = lexer_peek(l);
  Token t = token_make(l, TOKEN_STAR);

  if (c == '*') {
    lexer_advance(l);
    t = token_make(l, TOKEN_STAR_STAR);
  }
}

TokenResult lexer_next_token(Lexer *l) {
  if (lexer_is_at_end(l)) {
    return tr_token(
        token_create(TOKEN_EOF, lexer_str_view(l), l->line, l->column));
  }

  if (is_trash(lexer_peek(l)))
    lexer_trash(l);
  l->token_start = l->pos;
  l->token_line = l->line;
  l->token_column = l->column;

  char c = lexer_advance(l);

  if (is_identifier(c))
    return lexer_lex_identifier(l);

  if (isdigit(c))
    return lexer_lex_number(l);

  switch (c) {
  case '+':
    return lexer_lex_add(l);
  case '-':
    return lexer_lex_minus(l);
  case '=':
    return lexer_lex_equal(l);
  case '/':
    return lexer_lex_div(l);
  case '*':
    return lexer_lex_mul(l);
  case '(':
    return tr_token(token_make(l, TOKEN_LPAREN));
  case ')':
    return tr_token(token_make(l, TOKEN_RPAREN));
  case '{':
    return tr_token(token_make(l, TOKEN_LBRACE));
  case '}':
    return tr_token(token_make(l, TOKEN_RBRACE));
  default:
    return tr_error(
        lexer_error_create(SYNTAX_ERROR, l, "doesnt recognize that token"));
  }
}

ResultLexer create_result_lexer() {
  return (ResultLexer){.tokens = vector_create(sizeof(Token)),
                       .errors = vector_create(sizeof(LexerError))};
}

ResultLexer lexer_tokenize(Lexer *l) {
  init_keywords_if_needed();
  ResultLexer r = create_result_lexer();
  while (true) {
    TokenResult tr = lexer_next_token(l);
    lexer_trash(l);
    if (tr.kind == TR_ERROR) {
      vector_push(&r.errors, &tr.error);
      continue;
    }
    if (tr.kind == TR_TOKEN && tr.token.kind == TOKEN_EOF)
      break;

    if (tr.kind == TR_TOKEN && tr.token.kind == TOKEN_IDENTIFIER) {
      tr.token.kind = hashmap_contains(&keywords, &tr.token.raw)
                          ? *(TokenKind *)hashmap_get(&keywords, &tr.token.raw)
                          : tr.token.kind;
    }
    vector_push(&r.tokens, &tr.token);
  }

  return r;
}

void lexer_destroy(Lexer *l) { free(l->buffer); }
