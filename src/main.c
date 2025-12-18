#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parser/parser.h"
#include "utils/vector.h"
#include <stddef.h>
#include <stdio.h>

void debug_errors(Vector *errors, char *buffer) {
  for (size_t i = 0; i < errors->length; ++i) {
    LexerError err = *(LexerError *)vector_get(errors, i);
    printf("error: %s:\nline:%zu column:%zu\n %.*s\n", err.msg,
           err.line,
           err.column,
           (int)err.length, buffer + err.start);
  }
}

int main(int argc, char **argv) {

  if (argc == 1)
    return 0;

  for (int i = 1; i < argc; ++i) {
    FILE *f = fopen(argv[i], "r");
    Lexer l = lexer_create(f);
    ResultLexer r = lexer_tokenize(&l);
    if (r.errors.length) {
      debug_errors(&r.errors, l.buffer);
    }
    for (int j = 0; j < r.tokens.length; ++j) {
      debug_token((Token *)vector_get(&r.tokens, j));
    }

    Parser p;
    lexer_destroy(&l);
    vector_destroy(&r.errors);
  }
}
