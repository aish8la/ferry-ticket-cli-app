#ifndef FILES_H
#define FILES_H

#include "bookings.h"
#include "routes.h"

typedef enum {
  FILE_IO_OK = 0,
  FILE_IO_ERR_INVALID_INPUT,
  FILE_IO_ERR_OPEN,
  FILE_IO_ERR_ALLOC,
  FILE_IO_ERR_WRITE,
  FILE_IO_ERR_FORMAT
} file_io_error_t;

int save_routes_to_file(const char *filename, const RouteList *list);
int load_routes_from_file(const char *filename, RouteList *list);
int save_bookings_to_file(const char *filename, const BookingList *list);

#endif // !FILES_H
