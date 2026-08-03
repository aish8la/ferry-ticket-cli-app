#include "routes.h"
#include "utilities.h"
#include "validation.h"

int add_route(RouteList *list, int route_id, const char *departure_island,
              const char *destination_island, const char *departure_date,
              const char *departure_time, double price, int max_capacity) {
  FerryRoute new_route;

  // checks for empty strings and negative values
  if (list == NULL || !is_non_empty(destination_island) ||
      !is_non_empty(departure_island) || !is_positive_double(price) ||
      !is_positive_int(max_capacity)) {
    return ROUTE_ERR_INVALID_INPUT;
  }

  // checks and returns error if route exists
  if (route_id_exists(list, route_id)) {
    return ROUTE_ERR_DUPLICATE_ID;
  }

  new_route.route_id = route_id;
  copy_str(new_route.departure_island, departure_island, ROUTE_STR_LEN);
  copy_str(new_route.destination_island, destination_island, ROUTE_STR_LEN);
  copy_str(new_route.departure_date,
           departure_date != NULL ? departure_date : "", ROUTE_STR_LEN);
  copy_str(new_route.departure_time,
           departure_time != NULL ? departure_time : "", ROUTE_STR_LEN);
  new_route.price = price;
  new_route.max_capacity = max_capacity;
  new_route.available_seats = max_capacity;

  list->routes[list->count] = new_route;
  list->count++;
  return ROUTE_OK;
}

int search_route_by_id(const RouteList *list, int route_id) {
  int i;

  // return -1 if null pointer
  if (list == NULL) {
    return -1;
  }

  // searches the routes array by route_id and return the index of the element
  for (i = 0; i < list->count; i++) {
    if (list->routes[i].route_id == route_id) {
      return i;
    }
  }
  // returns -1 if not found
  return -1;
}

int route_id_exists(const RouteList *list, int route_id) {
  return search_route_by_id(list, route_id) != -1;
}
