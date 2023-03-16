#ifndef JF_READLINE_H
#define JF_READLINE_H

#include <stdio.h>

#include "string_builder.h"

// Read a line from the file pointer fp into the string builder sb.
// Returns true if a line was read, false if EOF was reached.
// If a line was read, the line is terminated with a null character, and
// does not include a carriage return or newline character.
extern int JF_freadline(FILE *fp, StringBuilder_T sb);

// Read a line from stdin into the string builder sb.
// Returns true if a line was read, false if EOF was reached.
// If a line was read, the line is terminated with a null character, and
// does not include a carriage return or newline character.
extern int JF_readline(StringBuilder_T sb);

#endif  // JF_READLINE_H