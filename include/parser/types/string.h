#ifndef HELPERS_H
#define HELPERS_H

#include <stddef.h>
typedef struct String String;

struct String {
  char* ptr;
  size_t len;
  size_t capacity;
};


String* string_from(char* ptr, size_t len);

// s1 + s2
void string_add(String* s1, String* s2);

//free string
void string_destroy(String* s);

#endif

