#include "bookings.h"
#include "routes.h"
#include "utilities.h"
#include "validation.h"
#include <string.h>

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
