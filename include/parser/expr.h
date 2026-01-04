#ifndef EXPR_H
#define EXPR_H

#include <stddef.h>
#include "parser/parser.h"
#include "utils/str_view.h"


typedef enum {
  PREC_NONE = 0,
  PREC_ASSIGN,     // =
  PREC_OR,         // ||
  PREC_AND,        // &&
  PREC_EQUALITY,   // == !=
  PREC_COMPARISON, // < > <= >=
  PREC_TERM,       // + -
  PREC_FACTOR,     // * /
  PREC_UNARY,      // ! -
  PREC_PRIMARY
} Precedence;


typedef enum {
  EXPR_INT,
  EXPR_FLOAT,
  EXPR_BINARY,
  EXPR_ERROR,
} ExprType;


typedef struct Expr Expr;
typedef struct BinaryExpr BinaryExpr;


struct Expr {
  ExprType type;
  union {
    long long int_value;
    double float_value;
    BinaryExpr *binary;
  } as;
};

struct BinaryExpr {
  Expr *left;
  TokenKind op; 
  Expr *right;
};


Expr *parse_primary_expr(Parser *p);
Expr *parse_expr(Parser *p, Precedence precedence);
Expr *parse_expression(Parser *p);


void debug_expr(Expr *expr);

#endif

