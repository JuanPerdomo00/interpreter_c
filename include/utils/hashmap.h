#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stddef.h>

typedef size_t (*HashFunc)(const void *);
typedef bool   (*CmpFunc)(const void *, const void *);

typedef enum {
    EMPTY,
    OCCUPIED
} EntryState;

typedef struct {
    void       *key;
    void       *value;
    size_t      value_size;
    EntryState  state;
} Entry;

typedef struct {
    Entry     *table;
    size_t     length;
    size_t     capacity;
    HashFunc   hash;
    CmpFunc    cmp;
} Hashmap;

Hashmap hashmap_create(HashFunc hash, CmpFunc cmp);
void    hashmap_destroy(Hashmap *hm);

void    hashmap_insert(Hashmap *hm,
                       void *key,
                       const void *value,
                       size_t value_size);

void   *hashmap_get(Hashmap *hm, void *key);

bool hashmap_contains(Hashmap *hm, void* key);

// helpers for string keys
size_t string_hash(const void *element);
bool   string_cmp(const void *a, const void *b);

#endif

