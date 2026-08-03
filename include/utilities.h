#ifndef UTILITIES_H
#define UTILITIES_H

#include <stddef.h>

void copy_str(char *dest, const char *src, size_t dest_size);
int read_line(char *buf, size_t size);
void trim_newline(char *str);

#endif // !UTILITIES_H
