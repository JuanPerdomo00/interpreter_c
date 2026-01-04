#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>
#include <stdlib.h>

typedef struct Vector Vector;


struct Vector {
  void* data;
  size_t size;
  size_t length;
  size_t capacity;
};


Vector vector_create(size_t size, size_t capacity);

void* vector_get(Vector* v, size_t index);

void vector_push(Vector* v, void* element);

void vector_set(Vector* v, size_t index, void* element);

void vector_destroy(Vector* v);


#endif
