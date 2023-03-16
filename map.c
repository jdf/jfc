#include "map.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T Map_T

struct bucket {
  void *key;
  void *value;
  struct bucket *next;
};

struct T {
  struct bucket **buckets;
  size_t size;
  struct Map_config type;
};

// One-at-a-time hash function.
static size_t jenkins_hash(T t, void *key, size_t len) {
  char *k = (char *)key;
  size_t hash = 0;
  for (size_t i = 0; i < len; i++) {
    hash += *(k++);
    hash += (hash << 10);
    hash ^= (hash >> 6);
  }
  hash += (hash << 3);
  hash ^= (hash >> 11);
  hash += (hash << 15);
  return hash & (t->size - 1);
}

static size_t string_hash_fn(T t, void *key) {
  return jenkins_hash(t, key, strlen((const char *)key));
}

static int string_eq_fn(void *a, void *b) {
  return strcmp((char *)a, (char *)b) == 0;
}

static size_t uint32_hash_fn(T t, void *key) {
  return jenkins_hash(t, key, sizeof(uint32_t));
}

static int uint32_eq_fn(void *a, void *b) {
  unsigned long *ua = a;
  unsigned long *ub = b;
  return *ua == *ub;
}

const struct Map_config Map_string_config = {"string", string_hash_fn,
                                             string_eq_fn};
const struct Map_config Map_uint32_config = {"uint32", uint32_hash_fn,
                                             uint32_eq_fn};

// https://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
static size_t round_up_power_of_two(size_t v) {
  v--;
  v |= v >> 1;
  v |= v >> 2;
  v |= v >> 4;
  v |= v >> 8;
  v |= v >> 16;
  v++;
  return v;
}

// Creates a new HashTable with the given number of buckets.
T Map_new(struct Map_config type, size_t size) {
  size = round_up_power_of_two(size);
  T t = (T)calloc(1, sizeof(struct T));
  t->type = type;
  t->buckets = (struct bucket **)calloc(size, sizeof(struct bucket *));
  t->size = size;
  return t;
}

void Map_free(T *t) {
  free((*t)->buckets);
  free(*t);
  *t = NULL;
}

// Does not manage storage for the given key and value.
void Map_set(T t, void *key, void *value) {
  size_t hash = t->type.hash(t, key);
  struct bucket *b = t->buckets[hash];
  struct bucket *nb = (struct bucket *)malloc(sizeof(struct bucket));
  nb->key = key;
  nb->value = value;
  nb->next = b;
  t->buckets[hash] = nb;
}

void *Map_get(T t, void *key) {
  unsigned long hash = t->type.hash(t, key);
  struct bucket *b = t->buckets[hash];
  while (b != NULL) {
    if (t->type.equals(b->key, key)) {
      return b->value;
    }
    b = b->next;
  }
  return NULL;
}

int Map_contains(T t, void *key) { return Map_get(t, key) != NULL; }

#undef T