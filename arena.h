#ifndef ARENA_H
#define ARENA_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdlib.h>

// Memory arena.
typedef struct Arena Arena;
struct Arena {
  void *buffer;  // Raw memory buffer
  size_t size;   // Size of the buffer in bytes
  size_t offset; // Current buffer offset
  Arena *next;   // Next arena in the chain in case of allocating more than buffer size
};

// Create memory arena with the buffer of `size` bytes.
// Returns `NULL` on error.
static inline Arena *arena_new(size_t size) {
  Arena *a = malloc(sizeof(Arena));
  if (!a) return NULL;
  a->size = size;
  a->buffer = malloc(size);
  if (!a->buffer) {
    free(a);
    return NULL;
  }
  a->offset = 0;
  a->next = NULL;
  return a;
}

// Free memory Arena.
// If Arena `a` has `a->next` Arena, free it recursively.
static inline void arena_free(Arena *a) {
  if (!a) return;
  if (a->buffer) free(a->buffer);
  if (a->next) arena_free(a->next);
  free(a);
}

// Allocate memory from arena.
// If arena `a` does not have enough space - it will allocate new `Arena` with the same size as Arena `a` (or `size` in
// case of the `size` > `a->size`), set it as `a->next` and allocate memory from it.
// Returns `NULL` on error.
static inline void *arena_alloc(Arena *a, size_t size) {
  if (!a) return NULL;
  Arena **curr = &a;
  if ((*curr)->offset + size > (*curr)->size) {
    while ((*curr)->next && (*curr)->offset + size > (*curr)->size) *curr = (*curr)->next;
    Arena *new = arena_new(size > (*curr)->size ? size : (*curr)->size);
    if (!new) return NULL;
    (*curr)->next = new;
  }
  void *ptr = (*curr)->buffer + (*curr)->offset;
  (*curr)->offset += size;
  return ptr;
}

// Reset memory arena.
static inline void arena_reset(Arena *a) {
  if (!a) return;
  a->offset = 0;
  arena_free(a->next);
}

#ifdef __cplusplus
}
#endif

#endif // ARENA_H
