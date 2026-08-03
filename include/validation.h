#ifndef VALIDATION_H
#define VALIDATION_H

int parse_int(const char *str, int *out);
int parse_double(const char *str, double *out);
int is_non_empty(const char *str);
int is_positive_int(int value);
int is_positive_double(double value);

#endif // !VALIDATION_H
