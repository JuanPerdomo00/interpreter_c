#ifndef PARSER_H
#define PARSER_H

#include <stdbool.h>
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "memory/arena_allocator.h"


typedef struct Parser Parser;

struct Parser {
  Lexer *lexer;              
  Token current;             
  ArenaAllocator *arena;
  bool error_reported;
};


Parser parser_create(Lexer *lexer);
void   parser_destroy(Parser *p);


bool   parser_is_at_end(Parser *p);
Token *parser_peek(Parser *p);
void   parser_advance(Parser *p);


void parser_error(Parser *p, const char *msg);


void parser_sync(Parser *p);

#endif

