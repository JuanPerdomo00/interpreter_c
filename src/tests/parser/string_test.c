#include "parser/types/string.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

void test1() {
  char *cmp = "hola mundo";
  String *s = string_from("hola mundo", 10);

  for (size_t i = 0; i < s->len; ++i) {
    assert(cmp[i] == s->ptr[i]);
  }
  free(s->ptr);
  free(s);
}

void test2() {

  char *result = "hola mundo";

  String *s1 = string_from("hola", 4);

  String *s2 = string_from(" mundo", 6);

  string_add(s1, s2);

  for (size_t i = 0; i < s1->len; ++i) {
    assert(result[i] == s1->ptr[i]);
  }

  string_destroy(s1);
  string_destroy(s2);
}

int main() {
  test1();
  test2();
  printf("se ejecuto correctamente\n");
  return 0;
}
