#include "files.h"
#include <stddef.h>
#include <stdio.h>

static int write_records(FILE *fp, const void *records, size_t record_size,
                         int count) {
  // writes the number of records to the file
  if (fwrite(&count, sizeof(int), 1, fp) != 1) {
    return FILE_IO_ERR_WRITE;
  }
  // if records exist thhen write them to the file
  if (count > 0 && fwrite(records, record_size, count, fp) != (size_t)count) {
    return FILE_IO_ERR_WRITE;
  }
  return FILE_IO_OK;
}

int save_routes_to_file(const char *filename, const RouteList *list) {
  FILE *fp;
  int result;

  // check for null pointer
  if (filename == NULL || list == NULL) {
    return FILE_IO_ERR_INVALID_INPUT;
  }

  // open file in binary write mode
  fp = fopen(filename, "wb");

  // check if file was openned successfully
  if (fp == NULL) {
    return FILE_IO_ERR_OPEN;
  }

  // write the records to the file
  result = write_records(fp, list->routes, sizeof(FerryRoute), list->count);
  // close the file after using
  fclose(fp);

  return result;
}

int load_routes_from_file(const char *filename, RouteList *list) {
  FILE *fp;
  int count;
  int i;
  FerryRoute route;

  if (filename == NULL || list == NULL) {
    return FILE_IO_ERR_INVALID_INPUT;
  }

  // open the file in binary read mode
  fp = fopen(filename, "rb");
  if (fp == NULL) {
    return FILE_IO_ERR_OPEN;
  }

  // read route count from file
  if (fread(&count, sizeof(int), 1, fp) != 1) {
    fclose(fp);
    return FILE_IO_ERR_FORMAT;
  }

  for (i = 0; i < count; i++) {
    // read the data in the file to route variable each struct at a time
    if (fread(&route, sizeof(FerryRoute), 1, fp) != 1) {
      break;
    }
    if (route_list_append(list, &route) != ROUTE_OK) {
      fclose(fp);
      return FILE_IO_ERR_ALLOC;
    }
  }

  fclose(fp);
  return FILE_IO_OK;
}

int save_bookings_to_file(const char *filename, const BookingList *list) {
  FILE *fp;
  int result;

  if (filename == NULL || list == NULL) {
    return FILE_IO_ERR_INVALID_INPUT;
  }

  fp = fopen(filename, "wb");
  if (fp == NULL) {
    return FILE_IO_ERR_OPEN;
  }

  result = write_records(fp, list->bookings, sizeof(Booking), list->count);
  fclose(fp);
  return result;
}
