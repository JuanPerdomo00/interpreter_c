#include "lexer/kind.h"
#include "utils/hashmap.h"
#include <string.h>
#include <stdio.h>

void test1() {
  // string string
  Hashmap h = hashmap_create(string_hash, string_cmp);
  int x = 5;
                 hashmap_insert(&h,"function",&x, sizeof(int));

  printf("valor guardado para function es: %d\n", *(int*)hashmap_get(&h,"function"));

  x = 20;
  printf("hashmap contiene \"function\" %s\n", hashmap_contains(&h,"function") ? "true": "false");
  
}

int main() {

  test1();
  return 0;
}
