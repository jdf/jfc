#include "vector.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#define T Vector_T

static const size_t kDefaultCap = 32;

struct T {
  size_t len;
  size_t el_size;
  size_t cap;
  Vector_destructor free;
  char *data;
};

size_t Vector_len(T t) { return t->len; }
size_t Vector_el_size(T t) { return t->el_size; }
void *Vector_data(T t) { return t->data; }

void _noop(void *el) {}
Vector_destructor Vector_noop = _noop;

T Vector_new(struct Vector_config config) {
  T t = (T)calloc(1, sizeof(struct T));
  t->cap = kDefaultCap;
  t->data = calloc(kDefaultCap, sizeof(char));
  t->len = 0;
  t->el_size = config.element_size;
  t->free = config.element_destructor;
  return t;
}

void Vector_free(T *t) {
  if (t && *t) {
    Vector_clear(*t);
    free((*t)->data);
    free(*t);
    *t = NULL;
  }
}

void Vector_clear(T t) { Vector_resize(t, 0); }

static void zero(T t, size_t index) {
  // Clear the new object lazily.
  memset(t->data + index * t->el_size, 0, t->el_size);
}

void *Vector_append(T t) {
  Vector_resize(t, t->len + 1);
  return Vector_at(t, t->len - 1);
}

void *Vector_at(T t, size_t index) {
  assert(index < t->len);
  return t->data + index * t->el_size;
}

// Sets the size of this Vector, zeroing any newly allocated memory,
// and destroying any elements currently living beyond the new size.
extern void Vector_resize(T t, size_t size) {
  if (size == t->len)
    return;
  if (size < t->len) {
    for (size_t i = size; i < t->len; i++) {
      t->free(Vector_at(t, i));
    }
    t->len = size;
    return;
  }
  size_t new_cap = t->cap;
  while (new_cap < size) {
    new_cap *= 2;
  }
  t->cap = new_cap;
  t->data = realloc(t->data, t->cap * t->el_size);
  memset(t->data + t->len * t->el_size, 0, (size - t->len) * t->el_size);
  t->len = size;
}

#undef T
