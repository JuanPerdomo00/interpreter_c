#ifndef STR_VIEW_H
#define STR_VIEW_H
#include <stddef.h>


typedef struct StrView StrView;

struct StrView {
  char* ptr;
  size_t len;
};


StrView strview_create(char* ptr, size_t len);

size_t strview_hash(const void *key);

bool strview_cmp(const void* a, const void* b);

#endif
