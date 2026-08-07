#include "bookings.h"
#include "routes.h"
#include "utilities.h"
#include "validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prototype for static functions
static int ensure_capacity(BookingList *list);

int book_ticket(BookingList *booking_list, RouteList *route_list,
                const char *passenger_name, const char *phone_number,
                int route_id, int num_tickets, double payment_amount,
                double *out_balance, int *out_booking_id) {
  FerryRoute *route;
  double total_price;
  Booking new_booking;

  if (booking_list == NULL || route_list == NULL ||
      !is_non_empty(passenger_name) || !is_positive_int(num_tickets) ||
      payment_amount < 0.0) {
    return BOOKING_ERR_INVALID_INPUT;
  }

  // get route by id
  route = route_list_get_by_id(route_list, route_id);
  if (route == NULL) {
    return BOOKING_ERR_ROUTE_NOT_FOUND;
  }
  // check if there is available seats for booking
  if (num_tickets > route->available_seats) {
    return BOOKING_ERR_INSUFFICIENT_SEATS;
  }

  // get total price
  total_price = num_tickets * route->price;
  // check if payment amount is sufficient
  if (payment_amount < total_price) {
    return BOOKING_ERR_INSUFFICIENT_PAYMENT;
  }
  // ensure capacity to ensure enough memory allocated
  if (!ensure_capacity(booking_list)) {
    return BOOKING_ERR_ALLOC;
  }

  // deduct availabel seats from route
  route_deduct_seats(route, num_tickets);

  // initialize garbage memory so that padding added by compiler does not
  // contain garbage.
  memset(&new_booking, 0, sizeof(new_booking));

  // assign given data to the new booking variable
  new_booking.booking_id = booking_list->next_booking_id;
  copy_str(new_booking.passenger_name, passenger_name, BOOKING_STR_LEN);
  copy_str(new_booking.phone_number, phone_number != NULL ? phone_number : "",
           BOOKING_STR_LEN);
  new_booking.route_id = route_id;
  new_booking.num_tickets = num_tickets;
  new_booking.payment_amount = payment_amount;
  new_booking.total_price = total_price;
  new_booking.status = BOOKING_STATUS_ACTIVE;

  // append to the end of the array
  booking_list->bookings[booking_list->count] = new_booking;
  booking_list->count++;
  // increment next booking id
  booking_list->next_booking_id++;

  if (out_balance != NULL) {
    // assign the remaining balance amount to the variable
    *out_balance = payment_amount - total_price;
  }
  // assigns the booking id to the variable
  if (out_booking_id != NULL) {
    *out_booking_id = new_booking.booking_id;
  }

  return BOOKING_OK;
}

static int ensure_capacity(BookingList *list) {
  int new_capacity;
  Booking *new_bookings;

  // return true if enough capacity
  if (list->count < list->capacity) {
    return 1;
  }

  new_capacity =
      (list->capacity == 0) ? BOOKING_INITIAL_CAPACITY : list->capacity * 2;
  new_bookings = (Booking *)realloc(list->bookings,
                                    (size_t)new_capacity * sizeof(Booking));
  if (new_bookings == NULL) {
    return 0;
  }

  list->bookings = new_bookings;
  list->capacity = new_capacity;
  return 1;
}

int booking_list_append(BookingList *list, const Booking *booking) {
  if (list == NULL || booking == NULL) {
    return BOOKING_ERR_INVALID_INPUT;
  }
  if (!ensure_capacity(list)) {
    return BOOKING_ERR_ALLOC;
  }
  // append booking data end of array
  list->bookings[list->count] = *booking;
  list->count++;
  // ensure bookings id is greater than or equal to next_booking_id in list and
  // assigne the next_booking_id value by booking_id + 1
  if (booking->booking_id >= list->next_booking_id) {
    list->next_booking_id = booking->booking_id + 1;
  }
  return BOOKING_OK;
}

void booking_list_init(BookingList *list) {
  list->bookings = NULL;
  list->count = 0;
  list->capacity = 0;
  list->next_booking_id = 1;
}

void booking_list_free(BookingList *list) {
  free(list->bookings);
  list->bookings = NULL;
  list->count = 0;
  list->capacity = 0;
}
