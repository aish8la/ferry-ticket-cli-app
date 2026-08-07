#include "routes.h"
#include "utilities.h"
#include "validation.h"
#include <stdlib.h>
#include <string.h>

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

static int ensure_capacity(RouteList *list) {
  int new_capacity;
  FerryRoute *new_routes;

  // check if current number of records is less than the capacity of the array
  // and returns 1 true
  if (list->count < list->capacity) {
    return 1;
  }

  // check if current capacity is 0 and initialize capacity if so. if not then
  // double the capacity
  new_capacity =
      (list->capacity == 0) ? ROUTE_INITIAL_CAPACITY : list->capacity * 2;

  // reallocate memory to fit the new capacity
  new_routes = (FerryRoute *)realloc(list->routes,
                                     (size_t)new_capacity * sizeof(FerryRoute));
  // if realloc fails then return 0 falsy
  if (new_routes == NULL) {
    return 0;
  }

  list->routes = new_routes;
  list->capacity = new_capacity;
  return 1;
}

int route_list_append(RouteList *list, const FerryRoute *route) {
  if (list == NULL || route == NULL) {
    return ROUTE_ERR_INVALID_INPUT;
  }
  // ensures that there is enough capacity to append new data
  if (!ensure_capacity(list)) {
    return ROUTE_ERR_ALLOC;
  }
  // dreferences and appends route data to the next empty slot in the array
  list->routes[list->count] = *route;
  list->count++;
  return ROUTE_OK;
}

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

  // ensure if there is enough space in the route array
  if (!ensure_capacity(list)) {
    return ROUTE_ERR_ALLOC;
  }

  // initialize the memory allocated for this variable removing garbage values
  // so that they don't get saved to file when compiler adds padding between
  // fields of struct
  memset(&new_route, 0, sizeof(new_route));

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

void route_deduct_seats(FerryRoute *route, int tickets) {
  if (route == NULL) {
    return;
  }
  // deduct available seats from route
  route->available_seats -= tickets;
  // ensure that availabel seats don't go below 0
  if (route->available_seats < 0) {
    route->available_seats = 0;
  }
}

FerryRoute *route_list_get_by_id(RouteList *list, int route_id) {
  int index;

  if (list == NULL) {
    return NULL;
  }

  index = search_route_by_id(list, route_id);
  // null if search fails or not found
  if (index == -1) {
    return NULL;
  }

  // reference to route struct
  return &list->routes[index];
}

void route_list_init(RouteList *list) {
  list->routes = NULL;
  list->count = 0;
  list->capacity = 0;
}

void route_list_free(RouteList *list) {
  free(list->routes);
  list->routes = NULL;
  list->count = 0;
  list->capacity = 0;
}

int update_route(RouteList *list, int route_id, const char *departure_island,
                 const char *destination_island, const char *departure_date,
                 const char *departure_time, double price, int max_capacity) {
  int index;
  int booked_seats;
  FerryRoute *route;

  if (list == NULL || !is_non_empty(destination_island) ||
      !is_non_empty(departure_island) || !is_positive_double(price) ||
      !is_positive_int(max_capacity)) {
    return ROUTE_ERR_INVALID_INPUT;
  }

  index = search_route_by_id(list, route_id);
  if (index == -1) {
    return ROUTE_ERR_NOT_FOUND;
  }

  // reference to the route struct in the list
  route = &list->routes[index];
  booked_seats = route->max_capacity - route->available_seats;
  // check to ensure that max_capacity cannot be modified to be lower than
  // already booked seats
  if (max_capacity < booked_seats) {
    return ROUTE_ERR_INVALID_INPUT;
  }

  copy_str(route->departure_island, departure_island, ROUTE_STR_LEN);
  copy_str(route->destination_island, destination_island, ROUTE_STR_LEN);
  copy_str(route->departure_date, departure_date != NULL ? departure_date : "",
           ROUTE_STR_LEN);
  copy_str(route->departure_time, departure_time != NULL ? departure_time : "",
           ROUTE_STR_LEN);
  route->price = price;
  route->max_capacity = max_capacity;
  route->available_seats = max_capacity - booked_seats;

  return ROUTE_OK;
}

int remove_route(RouteList *list, int route_id, int has_active_bookings) {
  int index;
  int i;

  if (list == NULL) {
    return ROUTE_ERR_INVALID_INPUT;
  }

  index = search_route_by_id(list, route_id);
  if (index == -1) {
    return ROUTE_ERR_NOT_FOUND;
  }
  // to ensure that routes with bookings cannot be removed
  if (has_active_bookings) {
    return ROUTE_ERR_HAS_ACTIVE_BOOKINGS;
  }

  // moves all routes after the route to be deleted to the left of the array
  // overwriting the previous route
  // still keeps a copy of the last route in the list in memory in the last
  // block but it won't be accessed since now count is updated to exclude that
  // element
  for (i = index; i < list->count - 1; i++) {
    list->routes[i] = list->routes[i + 1];
  }
  list->count--;
  return ROUTE_OK;
}
