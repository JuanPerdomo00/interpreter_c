#include "utils/hashmap.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DEFAULT_CAP 200
#define LOAD_FACTOR 0.7

static void hashmap_resize(Hashmap *hm);

Hashmap hashmap_create(HashFunc hash, CmpFunc cmp)
{
    Hashmap hm;
    hm.capacity = DEFAULT_CAP;
    hm.length   = 0;
    hm.hash     = hash;
    hm.cmp      = cmp;
    hm.table    = calloc(hm.capacity, sizeof(Entry));
    return hm;
}

void hashmap_destroy(Hashmap *hm)
{
    for (size_t i = 0; i < hm->capacity; i++) {
        if (hm->table[i].state == OCCUPIED) {
            free(hm->table[i].value);
        }
    }
    free(hm->table);
}

void hashmap_insert(Hashmap *hm,
                    void *key,
                    const void *value,
                    size_t value_size)
{
    if ((double)hm->length / hm->capacity >= LOAD_FACTOR) {
        hashmap_resize(hm);
    }

    size_t idx = hm->hash(key) % hm->capacity;

    while (hm->table[idx].state == OCCUPIED) {
        if (hm->cmp(hm->table[idx].key, key)) {
            memcpy(hm->table[idx].value, value, value_size);
            return;
        }
        idx = (idx + 1) % hm->capacity;
    }

    hm->table[idx].key = key;
    hm->table[idx].value = malloc(value_size);
    memcpy(hm->table[idx].value, value, value_size);
    hm->table[idx].value_size = value_size;
    hm->table[idx].state = OCCUPIED;
    hm->length++;
}

void *hashmap_get(Hashmap *hm, void *key)
{
    size_t idx = hm->hash(key) % hm->capacity;

    while (hm->table[idx].state == OCCUPIED) {
        if (hm->cmp(key, hm->table[idx].key)) {
            return hm->table[idx].value;
        }
        idx = (idx + 1) % hm->capacity;
    }
    return NULL;
}

bool hashmap_contains(Hashmap *hm, void *key) {
  return hashmap_get(hm,key) != NULL;
}


static void hashmap_resize(Hashmap *hm)
{
    Entry  *old_table = hm->table;
    size_t  old_cap   = hm->capacity;

    hm->capacity *= 2;
    hm->table = calloc(hm->capacity, sizeof(Entry));
    hm->length = 0;

    for (size_t i = 0; i < old_cap; i++) {
        if (old_table[i].state == OCCUPIED) {
            hashmap_insert(hm,
                           old_table[i].key,
                           old_table[i].value,
                           old_table[i].value_size);
            free(old_table[i].value);
        }
    }
    free(old_table);
}


size_t string_hash(const void *element)
{
    const char *str = element;
    size_t hash = 0;
    while (*str) {
        hash = hash * 31 + (unsigned char)*str++;
    }
    return hash;
}

bool string_cmp(const void *a, const void *b)
{
    if (!a || !b) return false;
    return strcmp((const char *)a, (const char *)b) == 0;
}

