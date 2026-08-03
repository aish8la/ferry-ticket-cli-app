#include "validation.h"
#include <ctype.h>
#include <stdlib.h>

int parse_int(const char *str, int *out) {
  char *endptr;
  long value;

  // checks for null pointers
  if (str == NULL || out == NULL) {
    return 0;
  }

  // checks if dereferenced str has leading whitespaces and moves the pointer
  // address until the pointer points to the first non whitespace chara
  while (isspace((unsigned char)*str)) {
    str++;
  }
  // checks if it is just a null terminator after all leading whitespaces and
  // returns 0 falsy if so
  // will return at this if the str is just whitespaces or empty
  if (*str == '\0') {
    return 0;
  }

  // coverts the string in str into a long integer of base 10
  value = strtol(str, &endptr, 10);
  // strtol will assign str pointer to endptr if no converion happened (like in
  // case it was only strings).
  if (endptr == str) {
    return 0;
  }

  // skips all leading whitespaces in *endptr
  while (isspace((unsigned char)*endptr)) {
    endptr++;
  }

  // if the character in endptr is not \0 then that means there are some other
  // values like characters which could not be converted to int
  // return falsy if that is the case
  if (*endptr != '\0') {
    return 0;
  }

  // typecase into an int value and assign it to *out
  *out = (int)value;
  return 1;
}

// sames as parse int except we use strod to get a double which we don't
// typecast
int parse_double(const char *str, double *out) {
  char *endptr;
  double value;

  if (str == NULL || out == NULL) {
    return 0;
  }

  while (isspace((unsigned char)*str)) {
    str++;
  }
  if (*str == '\0') {
    return 0;
  }

  value = strtod(str, &endptr);
  while (isspace((unsigned char)*endptr)) {
    endptr++;
  }
  if (*endptr != '\0') {
    return 0;
  }

  *out = value;
  return 1;
}

int is_non_empty(const char *str) {
  int i;

  // check for null pointer
  if (str == NULL) {
    return 0;
  }

  // loops over the string as long as a null terminator is not found and returns
  // 1 when a non whitespaces is found
  for (i = 0; str[i] != '\0'; i++) {
    if (!isspace((unsigned char)str[i])) {
      return 1;
    }
  }
  // returns 0 falsey when it's only whitespaces or empty
  return 0;
}
