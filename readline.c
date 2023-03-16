#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "string_builder.h"

int JF_freadline(FILE *fp, StringBuilder_T sb) {
  StringBuilder_reset(sb);
  while (1) {
    int c = fgetc(fp);
    if (c == -1) {
      if (feof(fp))
        return StringBuilder_length(sb) > 0;
      fprintf(stderr, "error reading from stdin\n");
      exit(EXIT_FAILURE);
    }
    if (c == '\n') {
      if (StringBuilder_at(sb, -1) == '\r') {
        StringBuilder_truncate(sb, StringBuilder_length(sb) - 1);
      }
      return StringBuilder_length(sb) > 0;
    }
    StringBuilder_appendc(sb, c);
  }
}

int JF_readline(StringBuilder_T sb) { return JF_freadline(stdin, sb); }