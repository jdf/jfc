#ifndef JDF_VECTOR_H
#define JDF_VECTOR_H

#include <stddef.h>
#include <stdint.h>

// An unboxed dynamic array.

#define T Vector_T

typedef struct T *T;

typedef void (*Vector_destructor)(void *el);

extern Vector_destructor Vector_noop;

struct Vector_config {
  size_t element_size;
  Vector_destructor element_destructor;
};

extern T Vector_new(struct Vector_config config);
extern void Vector_free(T *t);

extern size_t Vector_len(T t);
extern size_t Vector_el_size(T t);
extern void *Vector_data(T t);

// Sets the length of this Vector to 0, calling the destructor
// for each object currently in the Vector.
extern void Vector_clear(T t);

// Increases the length of this Vector by 1, and returns a pointer
// to the newly added object.
extern void *Vector_append(T t);

// Returns a pointer to the object at the given index. It is
// a checked runtime error to provide an index >= the length
// of this Vector. It's an unchecked runtime error
extern void *Vector_at(T t, size_t index);

// Sets the size of this Vector, zeroing any newly allocated memory,
// and destroying any elements currently living beyond the new size.
extern void Vector_resize(T t, size_t size);

#undef T

#endif // JDF_VECTOR_H