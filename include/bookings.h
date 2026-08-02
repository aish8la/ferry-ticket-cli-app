#ifndef BOOKINGS_H
#define BOOKINGS_H

// length of booking related char arrays
#define BOOKING_STR_LEN 64

// struct for booking data
typedef struct {
  int booking_id;
  char passenger_name[BOOKING_STR_LEN];
  char phone_number[BOOKING_STR_LEN];
  int route_id;
  int num_tickets;
  double payment_amount;
  double total_price;
  int status; /* BOOKING_STATUS_ACTIVE or BOOKING_STATUS_CANCELLED */
} Booking;

#endif // !BOOKINGS_H
