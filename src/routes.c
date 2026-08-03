#include "routes.h"
#include <string.h>

int add_route(RouteList *list, int route_id, const char *departure_island,
              const char *destination_island, const char *departure_date,
              const char *departure_time, double price, int max_capacity) {
  FerryRoute new_route;

  new_route.route_id = route_id;
  strncpy(new_route.departure_island, destination_island, ROUTE_STR_LEN - 1);
  strncpy(new_route.departure_island, departure_island, ROUTE_STR_LEN - 1);
  strncpy(new_route.destination_island, destination_island, ROUTE_STR_LEN - 1);
  strncpy(new_route.departure_date,
          departure_date != NULL ? departure_date : "", ROUTE_STR_LEN - 1);
  strncpy(new_route.departure_time,
          departure_time != NULL ? departure_time : "", ROUTE_STR_LEN - 1);
  new_route.price = price;
  new_route.max_capacity = max_capacity;
  new_route.available_seats = max_capacity;

  list->routes[list->count] = new_route;
  list->count++;
  return 0;
}
