#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H
#include "lexer/lexer.h"
#include "utils/str_view.h"

// util functions for the lexer
//

bool is_identifier(char c);

bool sv_eq(StrView sv, const char* str);



#endif
