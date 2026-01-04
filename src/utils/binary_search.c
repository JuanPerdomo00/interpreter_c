#include "utils/binary_search.h"
int binary_search_upper_bound(Vector* v, uint32_t value) {
  uint32_t l = 0, r = v->length - 1;

  while(l <= r) {
    uint32_t mid = l + (r - l) / 2;
    uint32_t value_mid = *(uint32_t*)vector_get(v,mid);
    if(value <= value_mid) {
      l = mid + 1;
    } else {
      r = mid;
    }
  }
  return l;
}
