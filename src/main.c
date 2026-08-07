#include <stdio.h>

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
static void print_booking_table(const BookingList *list);
static void print_booking_row(const Booking *b);
static void print_booking_header(void);

/* ================== admin function =============== */

static void admin_mode(RouteList *routes, BookingList *bookings);
static void admin_add_route(RouteList *routes, BookingList *bookings);
static void admin_update_route(RouteList *routes, BookingList *bookings);
static void admin_remove_route(RouteList *routes, BookingList *bookings);
static void admin_view_summary(const RouteList *routes,
                               const BookingList *bookings);

/* ================= passenger functions ============= */

static void passenger_mode(RouteList *routes, BookingList *bookings);
static void passenger_book_ticket(RouteList *routes, BookingList *bookings);
static void passenger_view_booking(const BookingList *bookings);
static void passenger_cancel_booking(RouteList *routes, BookingList *bookings);
/* ==================== other functions ================== */

static void save_all(const RouteList *routes, const BookingList *bookings);
static void seed_preset_data(RouteList *routes, BookingList *bookings);

/* ===================== shared functions ========================== */

static void search_routes(const RouteList *routes);
static void sort_routes(RouteList *routes, int allow_date_and_seats);

/* =================== main ===================== */
int main(void) {

  RouteList routes;
  BookingList bookings;
  int choice;
  int routes_load_result;

  route_list_init(&routes);
  booking_list_init(&bookings);

  routes_load_result = load_routes_from_file(ROUTES_FILE, &routes);
  load_bookings_from_file(BOOKINGS_FILE, &bookings);

  printf("Ferry Ticket Booking & Management System\n");

  if (routes_load_result == FILE_IO_ERR_OPEN) {
    seed_preset_data(&routes, &bookings);
    save_all(&routes, &bookings);
    printf("No existing data found -- loaded sample routes and "
           "bookings.\n");
  }

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
      // free up memory allocated for the list
      route_list_free(&routes);
      booking_list_free(&bookings);
      return 0;
    default:
      printf("Invalid choice. Please try again.\n");
      break;
    }
  }

  // free up memory allocated
  route_list_free(&routes);
  booking_list_free(&bookings);

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
      admin_update_route(routes, bookings);
      break;
    case 3:
      admin_remove_route(routes, bookings);
      break;
    case 4:
      print_route_table(routes);
      break;
    case 5:
      search_routes(routes);
      break;
    case 6:
      sort_routes(routes, 1);
      break;
    case 7:
      print_booking_table(bookings);
      break;
    case 8:
      admin_view_summary(routes, bookings);
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
  if (!prompt_nonempty_string("Departure island: ", departure,
                              sizeof(departure))) {
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

static void admin_update_route(RouteList *routes, BookingList *bookings) {
  int route_id;
  int index;
  char departure[ROUTE_STR_LEN];
  char destination[ROUTE_STR_LEN];
  char date[ROUTE_STR_LEN];
  char time_str[ROUTE_STR_LEN];
  double price;
  int capacity;
  int result;

  if (!prompt_int("Route ID to update: ", &route_id))
    return;

  index = search_route_by_id(routes, route_id);
  // if route not found
  if (index == -1) {
    printf("No route found with ID %d.\n", route_id);
    return;
  }

  // display current ditails
  printf("Current details:\n");
  print_route_header();
  print_route_row(&routes->routes[index]);
  printf("Enter new details:\n");

  // prompt new details
  if (!prompt_nonempty_string("Departure island: ", departure,
                              sizeof(departure)))
    return;
  if (!prompt_nonempty_string("Destination island: ", destination,
                              sizeof(destination)))
    return;
  if (!prompt_string("Departure date: ", date, sizeof(date)))
    return;
  if (!prompt_string("Departure time: ", time_str, sizeof(time_str)))
    return;
  if (!prompt_double("Ticket price: ", &price))
    return;
  if (!prompt_int("Maximum seat capacity: ", &capacity))
    return;

  // call update function to update the route in memory
  result = update_route(routes, route_id, departure, destination, date,
                        time_str, price, capacity);
  switch (result) {
  case ROUTE_OK:
    printf("Route %d updated successfully.\n", route_id);
    // save to file
    save_all(routes, bookings);
    break;
  case ROUTE_ERR_NOT_FOUND:
    printf("No route found with ID %d.\n", route_id);
    break;
  case ROUTE_ERR_INVALID_INPUT:
    printf("Invalid input, or new capacity is smaller than seats already "
           "booked.\n");
    break;
  default:
    printf("Could not update route.\n");
    break;
  }
}

static void admin_remove_route(RouteList *routes, BookingList *bookings) {
  int route_id;
  int active_count;
  int result;

  if (!prompt_int("Route ID to remove: ", &route_id))
    return;

  active_count = count_active_bookings_for_route(bookings, route_id);
  result = remove_route(routes, route_id, active_count > 0);

  switch (result) {
  case ROUTE_OK:
    printf("Route %d removed successfully.\n", route_id);
    save_all(routes, bookings);
    break;
  case ROUTE_ERR_NOT_FOUND:
    printf("No route found with ID %d.\n", route_id);
    break;
  case ROUTE_ERR_HAS_ACTIVE_BOOKINGS:
    printf("Cannot remove route %d: it has %d active booking(s). "
           "Cancel those bookings first.\n",
           route_id, active_count);
    break;
  default:
    printf("Could not remove route.\n");
    break;
  }
}

static void admin_view_summary(const RouteList *routes,
                               const BookingList *bookings) {
  BookingSummary summary;

  compute_booking_summary(routes->count, bookings, &summary);

  printf("---- Booking Summary ----\n");
  printf("Total ferry routes:      %d\n", summary.total_routes);
  printf("Total active bookings:   %d\n", summary.total_active_bookings);
  printf("Total tickets booked:    %d\n", summary.total_tickets_booked);
  printf("Total revenue collected: %.2f\n", summary.total_revenue);
  if (summary.most_booked_route_id == -1) {
    printf("Most-booked route: none yet\n");
  } else {
    printf("Most-booked route: ID %d (%d tickets)\n",
           summary.most_booked_route_id, summary.most_booked_route_tickets);
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
      search_routes(routes);
      break;
    case 3:
      sort_routes(routes, 0);
      break;
    case 4:
      passenger_book_ticket(routes, bookings);
      break;
    case 5:
      passenger_view_booking(bookings);
      break;
    case 6:
      passenger_cancel_booking(routes, bookings);
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

static void passenger_view_booking(const BookingList *bookings) {
  int booking_id;
  int index;

  if (!prompt_int("Booking ID: ", &booking_id))
    return;

  index = search_booking_by_id(bookings, booking_id);
  if (index == -1) {
    printf("No booking found with ID %d.\n", booking_id);
    return;
  }

  print_booking_header();
  print_booking_row(&bookings->bookings[index]);
}

static void passenger_cancel_booking(RouteList *routes, BookingList *bookings) {
  int booking_id;
  int result;

  if (!prompt_int("Booking ID to cancel: ", &booking_id))
    return;

  result = cancel_booking(bookings, routes, booking_id);
  switch (result) {
  case BOOKING_OK:
    printf("Booking %d cancelled. Seats have been released.\n", booking_id);
    save_all(routes, bookings);
    break;
  case BOOKING_ERR_NOT_FOUND:
    printf("No booking found with ID %d.\n", booking_id);
    break;
  case BOOKING_ERR_ALREADY_CANCELLED:
    printf("Booking %d has already been cancelled.\n", booking_id);
    break;
  default:
    printf("Could not cancel the booking.\n");
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

static void print_booking_header(void) {
  printf("%-10s %-16s %-14s %-8s %-8s %10s %10s %-10s\n", "BookingID",
         "Passenger", "Phone", "RouteID", "Tickets", "Paid", "Total", "Status");
}

static void print_booking_row(const Booking *b) {
  printf("%-10d %-16s %-14s %-8d %-8d %10.2f %10.2f %-10s\n", b->booking_id,
         b->passenger_name, b->phone_number, b->route_id, b->num_tickets,
         b->payment_amount, b->total_price,
         b->status == BOOKING_STATUS_ACTIVE ? "Active" : "Cancelled");
}

static void print_booking_table(const BookingList *list) {
  int i;

  if (list->count == 0) {
    printf("No bookings recorded.\n");
    return;
  }

  print_booking_header();
  for (i = 0; i < list->count; i++) {
    print_booking_row(&list->bookings[i]);
  }
}

/* ======================= helpers ========================= */

static void save_all(const RouteList *routes, const BookingList *bookings) {
  if (save_routes_to_file(ROUTES_FILE, routes) != FILE_IO_OK) {
    printf("Warning: could not save routes to %s.\n", ROUTES_FILE);
  }
  if (save_bookings_to_file(BOOKINGS_FILE, bookings) != FILE_IO_OK) {
    printf("Warning: could not save bookings to %s.\n", BOOKINGS_FILE);
  }
}

static void seed_preset_data(RouteList *routes, BookingList *bookings) {
  int booking_id;

  add_route(routes, 101, "Male", "Hulhumale", "2026-08-05", "07:00", 10.00,
            150);
  add_route(routes, 102, "Male", "Villimale", "2026-08-05", "08:00", 10.00,
            120);
  add_route(routes, 103, "Male", "Maafushi", "2026-08-05", "14:30", 35.00, 100);
  add_route(routes, 104, "Male", "Guraidhoo", "2026-08-06", "15:15", 35.00, 80);
  add_route(routes, 105, "Male", "Thulusdhoo", "2026-08-06", "09:30", 20.00,
            90);

  book_ticket(bookings, routes, "Aishath Nasheed", "+960 771-2345", 103, 2,
              70.00, NULL, &booking_id);
  book_ticket(bookings, routes, "Ibrahim Waheed", "+960 992-6781", 101, 1,
              10.00, NULL, &booking_id);
}

/* ===================== shared functions ========================== */

static void search_routes(const RouteList *routes) {
  int choice;
  int route_id;
  int index;
  char destination[ROUTE_STR_LEN];
  int indices[64];
  int found;
  int i;

  printf("Search by: 1) Route ID  2) Destination\n");
  if (!prompt_int("Choice: ", &choice)) {
    return;
  }

  // search and print route by id
  if (choice == 1) {
    if (!prompt_int("Route ID: ", &route_id)) {
      return;
    }
    index = search_route_by_id(routes, route_id);
    if (index == -1) {
      printf("No route found with ID %d.\n", route_id);
      return;
    }
    print_route_header();
    print_route_row(&routes->routes[index]);
    // search and print route by destination
  } else if (choice == 2) {
    if (!prompt_nonempty_string("Destination: ", destination,
                                sizeof(destination))) {
      return;
    }
    found = search_route_by_destination(routes, destination, indices, 64);
    if (found == 0) {
      printf("No routes found for destination \"%s\".\n", destination);
      return;
    }
    // print all routes found by destination
    print_route_header();
    for (i = 0; i < found && i < 64; i++) {
      print_route_row(&routes->routes[indices[i]]);
    }
  } else {
    printf("Invalid choice.\n");
  }
}

static void sort_routes(RouteList *routes, int allow_date_and_seats) {
  int field_choice;
  int order_choice;
  int ascending;

  // check if allowed to sort by date and seats (for admin and passenger mode)
  if (allow_date_and_seats) {
    printf("Sort by: 1) Price  2) Destination  3) Date  4) Available seats\n");
  } else {
    printf("Sort by: 1) Price  2) Destination\n");
  }
  if (!prompt_int("Choice: ", &field_choice)) {
    return;
  }

  printf("Order: 1) Ascending  2) Descending\n");
  if (!prompt_int("Choice: ", &order_choice)) {
    return;
  }

  // if order_choice is not 2 then it is ascending
  ascending = (order_choice != 2);

  switch (field_choice) {
  case 1:
    sort_routes_by_price(routes, ascending);
    break;
  case 2:
    sort_routes_by_destination(routes, ascending);
    break;
    // only allow the following if allow_date_and_seats is truthy
  case 3:
    if (allow_date_and_seats) {
      sort_routes_by_date(routes, ascending);
      break;
    }
    printf("Invalid choice.\n");
    return;
  case 4:
    if (allow_date_and_seats) {
      sort_routes_by_available_seats(routes, ascending);
      break;
    }
    printf("Invalid choice.\n");
    return;
  default:
    printf("Invalid choice.\n");
    return;
  }

  // print route table after successfully sorted
  print_route_table(routes);
}
