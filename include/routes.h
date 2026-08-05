#ifndef ROUTES_H
#define ROUTES_H

// length of route related strings
#define ROUTE_STR_LEN 64
// initial capacity of route array
#define ROUTE_INITIAL_CAPACITY 4

// struct for route data
typedef struct {
  int route_id;
  char departure_island[ROUTE_STR_LEN];
  char destination_island[ROUTE_STR_LEN];
  char departure_date[ROUTE_STR_LEN];
  char departure_time[ROUTE_STR_LEN];
  double price;
  int max_capacity;
  int available_seats;
} FerryRoute;

// struct to store info on dynamic array for ferry route structs
typedef struct {
  FerryRoute *routes;
  int count;
  int capacity;
} RouteList;

typedef enum {
  ROUTE_OK = 0,
  ROUTE_ERR_DUPLICATE_ID,
  ROUTE_ERR_INVALID_INPUT,
  ROUTE_ERR_NOT_FOUND,
  ROUTE_ERR_HAS_ACTIVE_BOOKINGS,
  ROUTE_ERR_ALLOC
} route_error_t;

int add_route(RouteList *list, int route_id, const char *departure_island,
              const char *destination_island, const char *departure_date,
              const char *departure_time, double price, int max_capacity);
int search_route_by_id(const RouteList *list, int route_id);
int route_id_exists(const RouteList *list, int route_id);
int route_list_append(RouteList *list, const FerryRoute *route);
void route_deduct_seats(FerryRoute *route, int tickets);

#endif // !ROUTES_H
