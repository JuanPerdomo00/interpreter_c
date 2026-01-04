#include "lexer/lexer.h"
#include "parser/expr.h"
#include "parser/parser.h"
#include <stdio.h>

/* TODO: src_map is alr created we need to implement
 * this in the lexer, quit from the lexer the line and column
 * and throw the errors with the new implementation of src_map*/

int main(int argc, char **argv) {

  if (argc <= 1)
    return 0;

  for (int i = 1; i < argc; ++i) {

    FILE *f = fopen(argv[i], "r");
    if (!f) {
      perror(argv[i]);
      continue;
    }


    Lexer lexer = lexer_create(argv[i], f);
    Parser parser = parser_create(&lexer);


    while (!parser_is_at_end(&parser)) {
      Expr *expr = parse_expression(&parser);
      // debug_expr(expr);
      // printf("\n");

      if (!expr) {
        parser_sync(&parser);
        continue;
      }


      arena_reset(lexer.arena);
      arena_reset(parser.arena);
    }
    parser_destroy(&parser);
    lexer_destroy(&lexer);
    fclose(f);
  }

  return 0;
}
