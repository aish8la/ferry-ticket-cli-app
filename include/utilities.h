#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>

// max size of input string buffer
#define LINE_BUF_SIZE 256

void copy_str(char *dest, const char *src, size_t dest_size);
int read_line(char *buf, size_t size);
void trim_newline(char *str);
int prompt_int(const char *prompt, int *out);
int prompt_string(const char *prompt, char *out, size_t size);

#endif // !UTILITIES_H
