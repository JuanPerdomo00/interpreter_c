#ifndef LEXER_H
#define LEXER_H
#include "lexer/errors.h"
#include "lexer/token.h"
#include "utils/vector.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct Lexer Lexer;


struct Lexer {
  char* buffer;
  size_t token_start;
  size_t token_line;
  size_t token_column;
  size_t length;
  size_t pos;
  size_t line;
  size_t column;
};



typedef struct ResultLexer ResultLexer;

struct ResultLexer {
  Vector tokens; // Token
  Vector errors; // LexerError
};

void result_lexer_destroy(ResultLexer* rl);


typedef enum TokenResultKind TokenResultKind;

enum TokenResultKind {
  TR_TOKEN,
  TR_ERROR, 
};

typedef struct TokenResult TokenResult;
struct TokenResult {
  TokenResultKind kind;
  union {
  Token token;
  LexerError error;
  };
};

Lexer lexer_create(FILE* file);



Token token_make(Lexer* l, TokenKind k);

TokenResult lexer_lex_identifier(Lexer *l);

TokenResult lexer_lex_number(Lexer* l);

TokenResult lexer_next_token(Lexer* l);

ResultLexer create_result_lexer();

// returns a vector of tokens
ResultLexer lexer_tokenize(Lexer* l);

void lexer_destroy(Lexer* l);


#endif
