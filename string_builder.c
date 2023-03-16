#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_builder.h"

#define T StringBuilder_T

struct T {
  char *buf;
  size_t len;
  size_t cap;
};

const size_t kDefaultSize = 128;

static void ensure_capacity(T sb, size_t len) {
  size_t new_cap = sb->cap;
  while (sb->len + len >= new_cap) {
    new_cap <<= 1;
    printf("new_cap: %zu\n", new_cap);
  }
  if (new_cap != sb->cap) {
    sb->buf = (char *)realloc(sb->buf, sb->cap);
  }
}

T StringBuilder_new(void) {
  T sb = (T)calloc(1, sizeof(*sb));
  sb->buf = (char *)malloc(kDefaultSize);
  sb->len = 0;
  sb->cap = kDefaultSize;
  sb->buf[0] = 0;
  return sb;
}

void StringBuilder_free(T *sb) {
  if (*sb == NULL) {
    return;
  }
  free((*sb)->buf);
  free(*sb);
  *sb = NULL;
}

void StringBuilder_append(T sb, const char *s, size_t len) {
  if (len == 0) {
    len = strlen(s);
  }
  ensure_capacity(sb, len);
  memcpy(sb->buf + sb->len, s, len);
  sb->len += len;
  sb->buf[sb->len] = 0;
}

void StringBuilder_appendc(T sb, char c) {
  ensure_capacity(sb, 1);
  sb->buf[sb->len++] = c;
  sb->buf[sb->len] = 0;
}

const char *StringBuilder_peek(T sb) { return sb->buf; }

void StringBuilder_reset(T sb) {
  sb->len = 0;
  sb->buf[0] = 0;
}

char *StringBuilder_build(T sb) {
  char *s = (char *)malloc(sb->len + 1);
  memcpy(s, sb->buf, sb->len + 1);
  return s;
}

char StringBuilder_at(T sb, int index) {
  if (index < 0) {
    index += sb->len;
  }
  if (index < 0 || index >= sb->len) {
    fprintf(stderr, "StringBuilder_at: index out of bounds: %d\n", index);
    exit(EXIT_FAILURE);
  }
  return sb->buf[index];
}

void StringBuilder_truncate(T sb, size_t len) {
  if (len < sb->len) {
    sb->len = len;
    sb->buf[len] = 0;
  }
}

size_t StringBuilder_length(T sb) { return sb->len; }

#undef T