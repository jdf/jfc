#ifndef JDF_MAP_H
#define JDF_MAP_H

#include <stddef.h>
#include <stdint.h>

#define T Map_T

typedef struct T *T;

typedef size_t (*Map_hash_fn)(T t, void *key);
typedef int (*Map_eq_fn)(void *a, void *b);

struct Map_config {
  const char *name;
  Map_hash_fn hash;
  Map_eq_fn equals;
};

extern const struct Map_config Map_uint32_config;
extern const struct Map_config Map_string_config;

// Construct a new Map_T.
extern T Map_new(struct Map_config config, size_t size);
extern void Map_free(T *t);

extern int Map_contains(T t, void *key);
extern void *Map_get(T t, void *key);
void Map_set(T t, void *key, void *value);

#undef T

#endif // JDF_MAP_H