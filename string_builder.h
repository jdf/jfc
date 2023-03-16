#ifndef JDF_STRING_BUILDER_H
#define JDF_STRING_BUILDER_H

#include <stddef.h>

#define T StringBuilder_T

typedef struct T *T;

// Construct a new string builder.
extern T StringBuilder_new(void);

// Free the given string builder.
extern void StringBuilder_free(T *sb);

// Append a string to the end of the string builder. If len is 0, this
// function will use strlen() to determine the length of the string.
extern void StringBuilder_append(T sb, const char *s, size_t len);

// Append a char to the end of the string builder.
extern void StringBuilder_appendc(T sb, char c);

// Return the character at the given index in the string builder.
// A negative index is interpreted as an offset from the end of the
// string builder.
// For example, if the string builder contains "abc\0", then
// StringBuilder_at(sb, 0) == 'a', and StringBuilder_at(sb, -1) == 'c'.
extern char StringBuilder_at(T sb, int index);

// Return the length of the string builder's buffer so far.
extern size_t StringBuilder_length(T sb);

// Sets the length of the string builder to the minimum of the given
// length and the current length of the string builder.
extern void StringBuilder_truncate(T sb, size_t len);

// Return a pointer to the string builder's internal buffer. The given
// string is null-terminated, but can be invalidated by any subsequent
// call to StringBuilder_append().
extern const char *StringBuilder_peek(T sb);

// Reset the string builder to an empty string.
extern void StringBuilder_reset(T sb);

// Construct a new string from the string builder's internal buffer.
// The caller is responsible for freeing the returned string.
extern char *StringBuilder_build(T sb);

#undef T

#endif  // JDF_STRING_BUILDER_H