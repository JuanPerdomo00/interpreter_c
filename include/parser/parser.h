#ifndef PARSER_H
#define PARSER_H
#include <stdbool.h>

#include "lexer/token.h"
#include "utils/vector.h"
typedef struct Parser Parser;


struct Parser {
  // Tokens
  Vector Tokens;
  size_t pos;
};


bool parser_is_at_end(Parser* p);

Token* parser_peek(Parser* p);

Token* parser_advance(Parser* p);


#endif
