#include "utils/vector.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

Vector vector_create(size_t size, size_t capacity) {
  return (Vector){
      .size = size, .data = malloc(size * capacity), .capacity = capacity, .length = 0};
}

void *vector_get(Vector *v, size_t index) {
  assert(index < v->length && "Index out of bounds");
  return (unsigned char *)v->data + index * v->size;
}

void vector_push(Vector *v, void *element) {
  if (v->length == v->capacity) {
    v->capacity *= 2;
    v->data = realloc(v->data, v->capacity * v->size);
  }
  void *dest = (unsigned char *)v->data + v->length * v->size;
  memcpy(dest, element, v->size);
  v->length++;
}

void vector_set(Vector *v, size_t index, void *element) {

  void *dest = vector_get(v, index);

  memcpy(dest, element, v->size);
}

void vector_destroy(Vector *v) {
  free(v->data);
}
