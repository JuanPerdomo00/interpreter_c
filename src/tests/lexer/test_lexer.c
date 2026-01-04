#include "lexer/lexer.h"
#include "lexer/token.h"
#include <stdio.h>


Token force_token(Lexer* l) {
  TokenResult t;
  while((t = lexer_next_token(l)).kind == TR_ERROR);

  return t.token;
}

int main(int argc, char* argv[]) {
  FILE* f = fopen(argv[1],"r");
  if(!f) {
    perror(argv[1]);
    return 1;
  }

  Lexer l = lexer_create(argv[1], f);

  Token t;
  while((t = force_token(&l)).kind != TOKEN_EOF) {
  }

  lexer_destroy(&l);

}
