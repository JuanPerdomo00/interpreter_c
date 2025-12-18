#ifndef ERRORS_H
#define ERRORS_H
#include <stddef.h>
typedef enum LexerErrorKind LexerErrorKind;

enum LexerErrorKind {
  SYNTAX_ERROR,
  UNKNOWN_ERROR,
};

typedef struct LexerError LexerError;

// typeError: desc: line: %d column: %d: tokens

struct LexerError {
  const char* msg;
  // describing the error
  size_t start;
  size_t length;
  size_t line;
  size_t column;
  LexerErrorKind kind;
};


void debug_lexer_error(LexerError* err, const char* buffer);


#endif
