#include "utilities.h"
#include "validation.h"
#include <stdio.h>
#include <string.h>

// function to copy from char array to another char array with a specified
// string length
void copy_str(char *dest, const char *src, size_t dest_size) {
  // - 1 leaves room for the null terminator as strncpy does not ensure null
  // terminator is added
  strncpy(dest, src, dest_size - 1);
  // \0 is null terminator added to the end of destination array
  dest[dest_size - 1] = '\0';
}

int read_line(char *buf, size_t size) {
  // reads stdin (standard input) and if no char is read or error occurs it will
  // return null causing the function to return 0 (falsy)
  if (fgets(buf, (int)size, stdin) == NULL) {
    return 0;
  }
  // checks if there is a newline chara in buf
  // if there is a new line that means that fgets above has read until newline
  // into the buf. so the if block code is skipped.
  if (strchr(buf, '\n') == NULL) {
    int c;
    // this loop will continue to run as long as there is no \n or EOF returned
    // by getchar.
    // getchar will consume anything left in stdin until it's cleared
    while ((c = getchar()) != '\n' && c != EOF) {
    }
  }

  trim_newline(buf);
  return 1;
}

void trim_newline(char *str) {
  size_t len;

  // checks whether the pointer is not a null pointer
  if (str == NULL) {
    return;
  }

  // gets the lenth of the string
  len = strlen(str);
  // checks if string is not of 0 length and that the last chara is a newline
  // chara
  if (len > 0 && str[len - 1] == '\n') {
    // if last chara is newline then replace it with null terminator \0
    str[len - 1] = '\0';
    len--;
  }
}

int prompt_int(const char *prompt, int *out) {
  char buf[LINE_BUF_SIZE];

  while (1) {
    printf("%s", prompt);
    // returns 0 falsy if error reading or reaching EOF
    if (!read_line(buf, sizeof(buf))) {
      return 0;
    }
    // returns 1 true if success in parsing to int
    if (parse_int(buf, out)) {
      return 1;
    }
    // if parse to int then ask again
    printf("Invalid number. Please enter digits only.\n");
  }
}

int prompt_string(const char *prompt, char *out, size_t size) {
  printf("%s", prompt);
  return read_line(out, size);
}

int prompt_double(const char *prompt, double *out) {
  char buf[LINE_BUF_SIZE];

  while (1) {
    printf("%s", prompt);
    if (!read_line(buf, sizeof(buf))) {
      return 0;
    }
    if (parse_double(buf, out)) {
      return 1;
    }
    printf("Invalid number. Please try again.\n");
  }
}

int prompt_nonempty_string(const char *prompt, char *out, size_t size) {
  while (1) {
    printf("%s", prompt);
    if (!read_line(out, size)) {
      return 0;
    }
    if (is_non_empty(out)) {
      return 1;
    }
    printf("This field cannot be empty. Please try again.\n");
  }
}
