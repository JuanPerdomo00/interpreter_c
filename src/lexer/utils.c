#include "lexer/utils.h"

bool is_identifier(char c) {
  if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
    return true;
  }
  return false;
}
