#include "lexer/utils.h"
#include "utils/str_view.h"
#include <stddef.h>
#include <stdbool.h>

bool is_identifier(char c) {
  if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
    return true;
  }
  return false;
}



bool sv_eq(StrView sv, const char* str) {
  for(size_t i = 0; i < sv.len; ++i) {
    if(sv.ptr[i] != str[i]) return false;
  }
  return true;
}

