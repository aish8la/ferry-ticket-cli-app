#include <stdio.h>

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
