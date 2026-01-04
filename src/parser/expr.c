#include "parser/expr.h"
#include "parser/helpers.h"
#include <assert.h>
#include <stdio.h>


static Expr convert_expr(ExprType type, void *value) {
  switch (type) {
    case EXPR_INT:
      return (Expr){ .type = type, .as.int_value = *(long long *)value };

    case EXPR_FLOAT:
      return (Expr){ .type = type, .as.float_value = *(double *)value };

    default:
      assert(0 && "unreachable ExprType");
  }
}

static Expr *create_expr(Parser *p, Expr expr) {
  return arena_push(p->arena, &expr, sizeof(Expr), alignof(Expr));
}


Expr *make_error_expr(Parser *p) {
  Expr *e = arena_push(p->arena, NULL, sizeof(Expr), alignof(Expr));
  e->type = EXPR_ERROR;
  return e;
}


Expr *parse_primary_expr(Parser *p) {
  if (parser_is_at_end(p))
    return NULL;

  Token *t = parser_peek(p);

  switch (t->kind) {
    case TOKEN_INT_LITERAL: {
      long long value = sv_to_int(t->raw);
      parser_advance(p);
      return create_expr(p, convert_expr(EXPR_INT, &value));
    }

    default:
      return NULL;
  }
}


Precedence kind_precedence(TokenKind kind) {
  switch (kind) {
    case TOKEN_EQUAL:
      return PREC_ASSIGN;

    case TOKEN_PLUS:
    case TOKEN_MINUS:
      return PREC_TERM;

    case TOKEN_STAR:
    case TOKEN_SLASH:
      return PREC_FACTOR;

    default:
      return PREC_NONE;
  }
}


Expr *make_binary_expr(Parser *p, Expr *left, TokenKind op, Expr *right) {
  BinaryExpr *binary =
      arena_push(p->arena, NULL, sizeof(BinaryExpr), alignof(BinaryExpr));

  binary->left  = left;
  binary->op    = op;
  binary->right = right;

  Expr *expr = arena_push(p->arena, NULL, sizeof(Expr), alignof(Expr));
  expr->type = EXPR_BINARY;
  expr->as.binary = binary;
  return expr;
}

Expr *parse_expr(Parser *p, Precedence pr) {
  Expr *left = parse_primary_expr(p);

  if (!left || left->type == EXPR_ERROR)
    return left;

  while (!parser_is_at_end(p)) {

     if (left->type == EXPR_ERROR)
      break;

    Token *op_tok = parser_peek(p);
    Precedence op_prec = kind_precedence(op_tok->kind);

    if (op_prec <= pr)
      break;

    TokenKind op = op_tok->kind;
    parser_advance(p); // consumir operador

    Expr *right = parse_expr(p, op_prec);

    if (!right || right->type == EXPR_ERROR) {
      parser_error(p, "expected expression after operator");
      return NULL;
    }

    left = make_binary_expr(p, left, op, right);
  }

  return left;
}

Expr *parse_expression(Parser *p) {
  return parse_expr(p, PREC_NONE);
}


void debug_expr(Expr *expr) {
  if (!expr) {
    printf("null");
    return;
  }

  switch (expr->type) {

    case EXPR_INT:
      printf("%lld", expr->as.int_value);
      break;

    case EXPR_FLOAT:
      printf("%f", expr->as.float_value);
      break;

    case EXPR_BINARY: {
      BinaryExpr *bin = expr->as.binary;
      printf("(");
      debug_expr(bin->left);
      // printf(" %s ", token_kind_to_string(bin->op));
      debug_expr(bin->right);
      printf(")");
      break;
    }

    case EXPR_ERROR:
      printf("<error>");
      break;

    default:
      break;
  }
}

