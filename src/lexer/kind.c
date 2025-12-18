#include "lexer/kind.h"

char *kind_debug(TokenKind kind) {
  switch (kind) {
    case TOKEN_INT_LITERAL:
    return "INTEGER";
    case TOKEN_FLOAT_LITERAL:
    return "FLOAT";
    case TOKEN_STRING_LITERAL:
    return "STRING";
    case TOKEN_IDENTIFIER:
    return "IDENTIFIER";
    case TOKEN_PLUS:
    return "PLUS";
    case TOKEN_MINUS:
    return "MINUS";
    case TOKEN_PLUS_PLUS:
    return "PLUS PLUS";
    case TOKEN_MINUS_MINUS:
    return "MINUS MINUS";
    case TOKEN_FUNC:
    return "FUNC";
    case TOKEN_IF:
    return "IF";
    case TOKEN_WHILE:
    return "WHILE";
    case TOKEN_ELSE:
    return "ELSE";
    case TOKEN_FOR:
    return "FOR";
    case TOKEN_LPAREN:
    return "LPAREN";
    case TOKEN_RETURN:
    return "RETURN";
  default:
    return "UNKNOWN";
  }
}
