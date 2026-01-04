#ifndef MEMORY_ARENA_ALLOCATOR_H
#define MEMORY_ARENA_ALLOCATOR_H

#include <stddef.h>
typedef struct ArenaAllocator ArenaAllocator;
typedef struct ArenaBlock ArenaBlock;

struct ArenaBlock {
  void *buf;
  size_t capacity;
  size_t offset;
  ArenaBlock *next;
};

struct ArenaAllocator {
  ArenaBlock *head;
  ArenaBlock *tail;
};

ArenaAllocator *arena_alloc_create(size_t capacity);

void *arena_push(ArenaAllocator *arena, const void *element, size_t size,
                 size_t aligment);

void arena_reset(ArenaAllocator *arena);

void arena_free(ArenaAllocator *arena);

#endif
