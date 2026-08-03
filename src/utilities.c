#include <string.h>

#include "utilities.h"

// function to copy from char array to another char array with a specified
// string length
void copy_str(char *dest, const char *src, size_t dest_size) {
  // - 1 leaves room for the null terminator as strncpy does not ensure null
  // terminator is added
  strncpy(dest, src, dest_size - 1);
  // \0 is null terminator added to the end of destination array
  dest[dest_size - 1] = '\0';
}
