#include "utils/str_view.h"
#include <stdlib.h>
#include <string.h>

StrView strview_create(char *ptr, size_t len) {
  return (StrView) {.ptr=ptr, .len=len};
}

size_t strview_hash(const void *key) {
    const StrView *s = key;
    size_t hash = 0;

    for (size_t i = 0; i < s->len; i++) {
        hash = hash * 31 + (unsigned char)s->ptr[i];
    }

    return hash;
}


bool strview_cmp(const void *a, const void *b) {
    const StrView *s1 = a;
    const StrView *s2 = b;

    if (s1->len != s2->len)
        return false;

    return memcmp(s1->ptr, s2->ptr, s1->len) == 0;
}
