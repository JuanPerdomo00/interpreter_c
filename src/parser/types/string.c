#include "parser/types/string.h"
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


static void string_resize(String* s, size_t new_capacity) {
  char* new_ptr = realloc(s->ptr,new_capacity);
  if(!new_ptr) {
    perror("not enough memory");
    exit(1);
    return;
  }

  s->ptr = new_ptr;
  s->capacity = new_capacity;
}

String *string_from(char *ptr, size_t len) {
  String *s = malloc(sizeof(String));

  s->ptr = malloc(len + 5);
  for (size_t i = 0; i < len; ++i) {
    s->ptr[i] = ptr[i];
  }

  s->len = len;
  s->capacity = len + 5;
  return s;
}


void string_add(String *s1, String *s2) {
  if(s1->len + s2->len >= s1->capacity) {
    string_resize(s1, s1->capacity * 2);
  }

  memcpy(s1->ptr + s1->len,s2->ptr,s2->len);
  s1->len += s2->len;
}


void string_destroy(String *s) {
  free(s->ptr);
  free(s);
}
