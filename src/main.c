#include <stdio.h>
#include <stdlib.h>

#include "bookings.h"
#include "files.h"
#include "routes.h"
#include "utilities.h"

// file name of route data
#define ROUTES_FILE "route.dat"
#define BOOKINGS_FILE "bookings.dat"
/* ----------------- function prototypes --------------- */

/* ================= display helpers =================== */

static void print_route_header(void);
static void print_route_row(const FerryRoute *r);
static void print_route_table(const RouteList *list);

/* ================== admin function =============== */

static void admin_mode(RouteList *routes, BookingList *bookings);
static void admin_add_route(RouteList *routes, BookingList *bookings);

/* ================= passenger functions ============= */

static void passenger_mode(RouteList *routes, BookingList *bookings);
static void passenger_book_ticket(RouteList *routes, BookingList *bookings);

/* ==================== other functions ================== */

static void save_all(const RouteList *routes, const BookingList *bookings);

/* =================== main ===================== */
int main(void) {

  RouteList routes = {
      .routes = malloc(sizeof(FerryRoute) * 10), .count = 0, .capacity = 10};
  BookingList bookings = {.bookings = malloc(sizeof(Booking)),
                          .capacity = 10,
                          .count = 0,
                          .next_booking_id = 0};
  int choice;

  load_routes_from_file(ROUTES_FILE, &routes);

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
      passenger_mode(&routes, &bookings);
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
    save_all(routes, bookings);
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
      print_route_table(routes);
      break;
    case 2:
      // search for route
      break;
    case 3:
      // sort routes
      break;
    case 4:
      passenger_book_ticket(routes, bookings);
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

static void passenger_book_ticket(RouteList *routes, BookingList *bookings) {
  char name[BOOKING_STR_LEN];
  char phone[BOOKING_STR_LEN];
  int route_id;
  int num_tickets;
  double payment;
  double balance;
  int booking_id;
  int result;

  if (!prompt_nonempty_string("Passenger name: ", name, sizeof(name)))
    return;
  if (!prompt_string("Phone number: ", phone, sizeof(phone)))
    return;
  if (!prompt_int("Route ID: ", &route_id))
    return;
  if (!prompt_int("Number of tickets: ", &num_tickets))
    return;
  if (!prompt_double("Payment amount: ", &payment))
    return;

  result = book_ticket(bookings, routes, name, phone, route_id, num_tickets,
                       payment, &balance, &booking_id);

  switch (result) {
  case BOOKING_OK: {
    int route_index = search_route_by_id(routes, route_id);
    printf("Booking confirmed!\n");
    printf("Booking ID:   %d\n", booking_id);
    printf("Passenger:    %s\n", name);
    if (route_index != -1) {
      printf("Route:        %s -> %s\n",
             routes->routes[route_index].departure_island,
             routes->routes[route_index].destination_island);
    }
    printf("Tickets:      %d\n", num_tickets);
    // calculate total amount paid using paynment and balance
    printf("Total paid:   %.2f\n", payment - balance);
    printf("Balance/change: %.2f\n", balance);
    save_all(routes, bookings);
    break;
  }
  case BOOKING_ERR_ROUTE_NOT_FOUND:
    printf("No route found with ID %d.\n", route_id);
    break;
  case BOOKING_ERR_INSUFFICIENT_SEATS:
    printf("Not enough seats available for that route.\n");
    break;
  case BOOKING_ERR_INSUFFICIENT_PAYMENT: {
    int route_index = search_route_by_id(routes, route_id);
    double total = (route_index != -1)
                       ? num_tickets * routes->routes[route_index].price
                       : 0.0;
    printf("Insufficient payment. Total due is %.2f, you paid %.2f (shortage: "
           "%.2f).\n",
           total, payment, total - payment);
    break;
  }
  case BOOKING_ERR_INVALID_INPUT:
    printf("Invalid input: passenger name must be non-empty and ticket count "
           "must be positive.\n");
    break;
  default:
    printf("Could not complete the booking.\n");
    break;
  }
}

/* ================== display helpers ================= */
// helper to print table header for route table
static void print_route_header(void) {
  printf("%-6s %-14s %-14s %-12s %-8s %10s %8s %8s\n", "ID", "From", "To",
         "Date", "Time", "Price", "MaxCap", "AvailSt");
}

// helper to print the row for route table
static void print_route_row(const FerryRoute *r) {
  printf("%-6d %-14s %-14s %-12s %-8s %10.2f %8d %8d\n", r->route_id,
         r->departure_island, r->destination_island, r->departure_date,
         r->departure_time, r->price, r->max_capacity, r->available_seats);
}

// print the route table from the route list
static void print_route_table(const RouteList *list) {
  int i;

  // if count of routes is 0 then print not available
  if (list->count == 0) {
    printf("No routes available.\n");
    return;
  }

  // print header and loop over route all routes in the array and print row for
  // each route
  print_route_header();
  for (i = 0; i < list->count; i++) {
    print_route_row(&list->routes[i]);
  }
}

static void save_all(const RouteList *routes, const BookingList *bookings) {
  if (save_routes_to_file(ROUTES_FILE, routes) != FILE_IO_OK) {
    printf("Warning: could not save routes to %s.\n", ROUTES_FILE);
  }
  if (save_bookings_to_file(BOOKINGS_FILE, bookings) != FILE_IO_OK) {
    printf("Warning: could not save bookings to %s.\n", BOOKINGS_FILE);
  }
}
