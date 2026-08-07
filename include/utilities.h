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
int prompt_double(const char *prompt, double *out);
int prompt_nonempty_string(const char *prompt, char *out, size_t size);
int str_equal_ci(const char *a, const char *b);

#endif // !UTILITIES_H
