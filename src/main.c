#include <stdio.h>
#include <stdlib.h>

#include "bookings.h"
#include "files.h"
#include "routes.h"
#include "utilities.h"

/* ----------------- function prototypes --------------- */

/* ================== admin function =============== */

static void admin_mode(RouteList *routes, BookingList *bookings);
static void admin_add_route(RouteList *routes, BookingList *bookings);

/* ================= passenger functions ============= */

static void passenger_mode(RouteList *routes, BookingList *bookings);

/* =================== main ===================== */
int main(void) {

  RouteList routes = {
      .routes = malloc(sizeof(FerryRoute) * 10), .count = 0, .capacity = 10};
  BookingList bookings;
  int choice;

  printf("Ferry Ticket Booking & Management System\n");

  while (1) {
    printf("\n1. Admin Mode\n2. Passenger Mode\n3. Exit\n");

    if (!prompt_int("Choice: ", &choice)) {
      break;
    }

    switch (choice) {
    case 1:
      admin_mode(&routes, &bookings);
      break;
    case 2:
      printf("passenger");
      break;
    case 3:
      printf("Goodbye.\n");
      return 0;
    default:
      printf("Invalid choice. Please try again.\n");
      break;
    }
  }

  return 0;
}

/* --------------------- admin mode --------------------------- */

static void admin_mode(RouteList *routes, BookingList *bookings) {
  int choice;

  while (1) {
    printf("\n---- Admin Mode ----\n");
    printf("1. Add a new ferry route\n");
    printf("2. Update an existing route\n");
    printf("3. Remove a route\n");
    printf("4. View all ferry routes\n");
    printf("5. Search for a route\n");
    printf("6. Sort routes\n");
    printf("7. View all passenger bookings\n");
    printf("8. View booking summary\n");
    printf("9. Return to main menu\n");

    if (!prompt_int("Choice: ", &choice)) {
      return;
    }

    switch (choice) {
    case 1:
      admin_add_route(routes, bookings);
      break;
    case 2:
      // update route
      break;
    case 3:
      // remove rout
      break;
    case 4:
      // view all routes
      break;
    case 5:
      // search route
      break;
    case 6:
      // sort routes
      break;
    case 7:
      // view all bookings
      break;
    case 8:
      // summary
      break;
    case 9:
      return;
    default:
      printf("Invalid choice. Please try again.\n");
      break;
    }
  }
}

static void admin_add_route(RouteList *routes, BookingList *bookings) {
  int route_id;
  char departure[ROUTE_STR_LEN];
  char destination[ROUTE_STR_LEN];
  char date[ROUTE_STR_LEN];
  char time_str[ROUTE_STR_LEN];
  double price;
  int capacity;
  int result;

  if (!prompt_int("Route ID: ", &route_id)) {
    return;
  }
  if (!prompt_string("Departure island: ", departure, sizeof(departure))) {
    return;
  }
  if (!prompt_nonempty_string("Destination island: ", destination,
                              sizeof(destination))) {
    return;
  }
  if (!prompt_string("Departure date: ", date, sizeof(date))) {
    return;
  }
  if (!prompt_string("Departure time: ", time_str, sizeof(time_str))) {
    return;
  }
  if (!prompt_double("Ticket price: ", &price)) {
    return;
  }
  if (!prompt_int("Maximum seat capacity: ", &capacity)) {
    return;
  }

  result = add_route(routes, route_id, departure, destination, date, time_str,
                     price, capacity);

  switch (result) {
  case ROUTE_OK:
    printf("Route %d added successfully.\n", route_id);
    save_routes_to_file("routes.dat", routes);
    break;
  case ROUTE_ERR_DUPLICATE_ID:
    printf("A route with ID %d already exists.\n", route_id);
    break;
  case ROUTE_ERR_INVALID_INPUT:
    printf("Invalid input: destination must be non-empty, price and capacity "
           "must be positive.\n");
    break;
  case ROUTE_ERR_ALLOC:
    printf("Error: could not allocate memory for the new route.\n");
    break;
  default:
    printf("Could not add route.\n");
    break;
  }
}

/* ----------------- passender mode -------------- */
static void passenger_mode(RouteList *routes, BookingList *bookings) {
  int choice;

  while (1) {
    printf("\n---- Passenger Mode ----\n");
    printf("1. View available ferry routes\n");
    printf("2. Search for a route\n");
    printf("3. Sort routes\n");
    printf("4. Book ticket(s)\n");
    printf("5. View a booking confirmation\n");
    printf("6. Cancel a booking\n");
    printf("7. Return to main menu\n");

    if (!prompt_int("Choice: ", &choice)) {
      return;
    }

    switch (choice) {
    case 1:
      // view abailable routes
      break;
    case 2:
      // search for route
      break;
    case 3:
      // sort routes
      break;
    case 4:
      // book ticket
      break;
    case 5:
      // view booking
      break;
    case 6:
      // cancel booking
      break;
    case 7:
      return;
    default:
      printf("Invalid choice. Please try again.\n");
      break;
    }
  }
}
