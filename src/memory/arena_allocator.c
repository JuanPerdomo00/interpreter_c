#include "memory/arena_allocator.h"
#include <stdlib.h>
#include <string.h>

static ArenaBlock *block_allocate(size_t capacity) {
  ArenaBlock *block = malloc(sizeof(ArenaBlock));
  block->capacity = capacity;
  block->offset = 0;
  block->buf = malloc(capacity);
  block->next = NULL;
  return block;
}

static void block_free(ArenaBlock *block) {
  free(block->buf);
  free(block);
}

ArenaAllocator *arena_alloc_create(size_t capacity) {
  ArenaAllocator *arena = malloc(sizeof(ArenaAllocator));

  ArenaBlock *block = block_allocate(capacity);

  arena->head = block;
  arena->tail = block;

  return arena;
}

void *arena_push(ArenaAllocator *arena, const void *element, size_t size,
                 size_t alignment) {
  ArenaBlock *tail = arena->tail;
  size_t aligned = (tail->offset + (alignment - 1)) & ~(alignment - 1);

  if (aligned + size > tail->capacity) {
    tail->next = block_allocate(tail->capacity);
    tail = tail->next;
    arena->tail = tail;
    aligned = 0;
  }

  void *tmp = (unsigned char *)tail->buf + aligned;
  if(element) {
    memcpy(tmp, element, size);
  }

  tail->offset = aligned + size;

  return tmp;
}

void arena_reset(ArenaAllocator *arena) {
  // forget about all the past pointers of the current chunk
  arena->tail->offset = 0;
}

void arena_free(ArenaAllocator *arena) {
  ArenaBlock *current = arena->head;
  while (current != NULL) {
    ArenaBlock *tmp = current;
    current = current->next;
    block_free(tmp);
  }
  free(arena);
}
