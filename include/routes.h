#ifndef ROUTES_H
#define ROUTES_H

// length of route related strings
#define ROUTE_STR_LEN 64

// struct for route data
typedef struct {
  int route_id;
  char departure_island[ROUTE_STR_LEN];
  char destination_island[ROUTE_STR_LEN];
  char departure_date[ROUTE_STR_LEN];
  char departure_time[ROUTE_STR_LEN];
  double price;
  int max_capacity;
  int available_seats;
} FerryRoute;

#endif // !ROUTES_H
