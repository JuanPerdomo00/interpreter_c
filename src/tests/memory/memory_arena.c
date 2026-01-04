#include <stdio.h>
#include <assert.h>
#include "memory/arena_allocator.h"

void test1() {
  ArenaAllocator* arena = arena_alloc_create(2);
  char a = 'a';
  char* ptr_a = arena_push(arena, &a, 1, 1);
  char b  = 'b';
  char* ptr_b = arena_push(arena, &b, 1, 1);

  assert(*ptr_a == 'a' && *ptr_b == 'b');
  arena_free(arena);
}


int main() {
  test1();
  return 0;
}
