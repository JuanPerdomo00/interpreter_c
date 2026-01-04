#ifndef PARSER_ERRORS_H
#define PARSER_ERRORS_H

#include "lexer/token.h"
#include <stdint.h>

typedef struct {
  uint32_t line;
  uint32_t column;
} ExprError;





#endif
