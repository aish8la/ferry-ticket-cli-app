#ifndef BOOKINGS_H
#define BOOKINGS_H

#include "routes.h"

// length of booking related char arrays
#define BOOKING_STR_LEN 64
// initial capacity of array
#define BOOKING_INITIAL_CAPACITY 4

typedef enum {
  BOOKING_STATUS_ACTIVE = 0,
  BOOKING_STATUS_CANCELLED = 1
} booking_status_t;

typedef enum {
  BOOKING_OK = 0,
  BOOKING_ERR_INVALID_INPUT,
  BOOKING_ERR_ROUTE_NOT_FOUND,
  BOOKING_ERR_INSUFFICIENT_SEATS,
  BOOKING_ERR_INSUFFICIENT_PAYMENT,
  BOOKING_ERR_NOT_FOUND,
  BOOKING_ERR_ALREADY_CANCELLED,
  BOOKING_ERR_ALLOC
} booking_error_t;

// struct for booking data
typedef struct {
  int booking_id;
  char passenger_name[BOOKING_STR_LEN];
  char phone_number[BOOKING_STR_LEN];
  int route_id;
  int num_tickets;
  double payment_amount;
  double total_price;
  booking_status_t
      status; /* BOOKING_STATUS_ACTIVE or BOOKING_STATUS_CANCELLED */
} Booking;

// struct to store dynamic array data and next id for booking structs
typedef struct {
  Booking *bookings;
  int count;
  int capacity;
  int next_booking_id;
} BookingList;

typedef struct {
  int total_routes;
  int total_active_bookings;
  int total_tickets_booked;
  double total_revenue;
  int most_booked_route_id;      // -1 if there are no active bookings
  int most_booked_route_tickets; // ticket count for most_booked_route_id
} BookingSummary;

int book_ticket(BookingList *booking_list, RouteList *route_list,
                const char *passenger_name, const char *phone_number,
                int route_id, int num_tickets, double payment_amount,

                double *out_balance, int *out_booking_id);
int booking_list_append(BookingList *list, const Booking *booking);
void booking_list_init(BookingList *list);
void booking_list_free(BookingList *list);
int count_active_bookings_for_route(const BookingList *list, int route_id);
void compute_booking_summary(int total_routes, const BookingList *bookings,
                             BookingSummary *out_summary);

#endif // !BOOKINGS_H
