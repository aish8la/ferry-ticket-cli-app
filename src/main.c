#include <stdio.h>

#include "bookings.h"
#include "routes.h"

/* ----------------- function prototypes --------------- */

/* ================== admin function =============== */

static void admin_mode(RouteList *routes, BookingList *bookings);

/* ================= passenger functions ============= */

static void passenger_mode(RouteList *routes, BookingList *bookings);

int main(void) {
  int choice;

  printf("Ferry Ticket Booking & Management System\n");

  while (1) {
    printf("\n1. Admin Mode\n2. Passenger Mode\n3. Exit\n");

    printf("Choice: ");
    scanf("%d", &choice);

    switch (choice) {
    case 1:
      printf("admin");
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
  }

  printf("select option:");
  scanf("%d", &choice);

  switch (choice) {
  case 1:
    // add route
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

    printf("select option:");
    scanf("%d", &choice);

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
