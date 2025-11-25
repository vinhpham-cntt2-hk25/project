
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TRIPS 100
#define MAX_TICKETS 500
#define STR_LEN 100

typedef struct Trip {
    int tripId;
    char destination[50];
    char time[50];
    int totalSeats;
    int bookedSeats;
    double fare; 
} Trip;

typedef struct Ticket {
    int ticketId;
    int tripId; 
    char passengerName[50];
    int seats;
    int paymentStatus; 
    char status[20];  
} Ticket;

Trip* trips[MAX_TRIPS];
Ticket* tickets[MAX_TICKETS];
int tripCount = 0;
int ticketCount = 0;


void flush_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void read_line(char* buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        size_t ln = strlen(buffer) - 1;
        if (ln >= 0 && buffer[ln] == '\n') buffer[ln] = '\0';
    } else {
        buffer[0] = '\0';
    }
}


void trim(char *s) {
    char *p = s;
    while (isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);
    int i = strlen(s) - 1;
    while (i >= 0 && isspace((unsigned char)s[i])) { s[i] = '\0'; i--; }
}

int findTripIndexById(int tripId) {
    for (int i = 0; i < tripCount; ++i) {
        if (trips[i] != NULL && trips[i]->tripId == tripId) return i;
    }
    return -1;
}

int findTicketIndexById(int ticketId) {
    for (int i = 0; i < ticketCount; ++i) {
        if (tickets[i] != NULL && tickets[i]->ticketId == ticketId) return i;
    }
    return -1;
}


int addTrip(int tripId, const char* destination, const char* time, int totalSeats, double fare) {
    if (tripCount >= MAX_TRIPS) return -2;
    if (totalSeats <= 0) return -3; 
    if (findTripIndexById(tripId) != -1) return -1; 

    Trip* t = (Trip*) malloc(sizeof(Trip));
    if (!t) return -4;
    t->tripId = tripId;
    strncpy(t->destination, destination, sizeof(t->destination)-1); t->destination[sizeof(t->destination)-1] = '\0';
    strncpy(t->time, time, sizeof(t->time)-1); t->time[sizeof(t->time)-1] = '\0';
    t->totalSeats = totalSeats;
    t->bookedSeats = 0;
    t->fare = fare;

    trips[tripCount++] = t;
    return 0;
}

int updateTrip(int tripId, const char* destination, const char* time, int totalSeats, double fare) {
    int idx = findTripIndexById(tripId);
    if (idx == -1) return -1; 
    Trip* t = trips[idx];

    if (totalSeats < t->bookedSeats) return -2;
    strncpy(t->destination, destination, sizeof(t->destination)-1); t->destination[sizeof(t->destination)-1] = '\0';
    strncpy(t->time, time, sizeof(t->time)-1); t->time[sizeof(t->time)-1] = '\0';
    t->totalSeats = totalSeats;
    t->fare = fare;
    return 0;
}


void listTrips() {
    if (tripCount == 0) {
        printf("There are no flights yet.\n");
        return;
    }
    printf("tripList:\n");
    printf("ID\tDestination\tTime\t\tTotal\tBooked\tFare\n");
    for (int i = 0; i < tripCount; ++i) {
        Trip* t = trips[i];
        if (t) {
            printf("%d\t%s\t\t%s\t%d\t%d\t%.2f\n",
                t->tripId, t->destination, t->time, t->totalSeats, t->bookedSeats, t->fare);
        }
    }
}


int bookTicket(int ticketId, int tripId, const char* passengerName, int seats) {
    if (ticketCount >= MAX_TICKETS) return -4; 
    if (seats <= 0) return -3;
    if (findTicketIndexById(ticketId) != -1) return -1; 
    int tidx = findTripIndexById(tripId);
    if (tidx == -1) return -2; 
    Trip* t = trips[tidx];
    if (t->bookedSeats + seats > t->totalSeats) return -5; 

    Ticket* tk = (Ticket*) malloc(sizeof(Ticket));
    if (!tk) return -6; 
    tk->ticketId = ticketId;
    tk->tripId = tripId;
    strncpy(tk->passengerName, passengerName, sizeof(tk->passengerName)-1); tk->passengerName[sizeof(tk->passengerName)-1] = '\0';
    tk->seats = seats;
    tk->paymentStatus = 0;
    strncpy(tk->status, "active", sizeof(tk->status)-1); tk->status[sizeof(tk->status)-1] = '\0';

    tickets[ticketCount++] = tk;
    t->bookedSeats += seats;
    return 0;
}

void checkTicket(int ticketId) {
    int idx = findTicketIndexById(ticketId);
    if (idx == -1) {
        printf("Ticket ID %d không t?n t?i.\n", ticketId);
        return;
    }
    Ticket* tk = tickets[idx];
    printf("Thông tin vé:\n");
    printf("Ticket ID: %d\n", tk->ticketId);
    printf("Trip ID: %d\n", tk->tripId);
    printf("Passenger: %s\n", tk->passengerName);
    printf("Seats: %d\n", tk->seats);
    printf("Payment status: %s\n", tk->paymentStatus ? "PAID" : "UNPAID");
    printf("Status: %s\n", tk->status);
}

int payTicket(int ticketId) {
    int idx = findTicketIndexById(ticketId);
    if (idx == -1) return -1; 
    Ticket* tk = tickets[idx];
    if (strcmp(tk->status, "active") != 0) return -2; 
    if (tk->paymentStatus == 1) return -3;
    return 0;
}

int lockTicket(int ticketId) {
    int idx = findTicketIndexById(ticketId);
    if (idx == -1) return -1;
    Ticket* tk = tickets[idx];
    if (strcmp(tk->status, "active") != 0) return -2;
    tk->status[sizeof(tk->status)-1] = '\0';
    return 0;
}

int cancelTicket(int ticketId) {
    int idx = findTicketIndexById(ticketId);
    if (idx == -1) return -1;
    Ticket* tk = tickets[idx];
    if (strcmp(tk->status, "active") != 0) return -2; 
    if (tk->paymentStatus == 1) return -3; 

    int tindex = findTripIndexById(tk->tripId);
    if (tindex != -1) {
        Trip* t = trips[tindex];
        t->bookedSeats -= tk->seats;
        if (t->bookedSeats < 0) t->bookedSeats = 0;
    }
    strncpy(tk->status, "cancelled", sizeof(tk->status)-1);
    tk->status[sizeof(tk->status)-1] = '\0';
    return 0;
}

void reportStatistics() {
    int totalBooked = 0, totalPaid = 0, totalCancelled = 0;
    double revenue = 0.0;
    for (int i = 0; i < ticketCount; ++i) {
        Ticket* tk = tickets[i];
        if (!tk) continue;
        totalBooked += tk->seats;
        if (tk->paymentStatus == 1 && strcmp(tk->status,"cancelled") != 0) {
            totalPaid += tk->seats;
            int tindex = findTripIndexById(tk->tripId);
            if (tindex != -1) {
                Trip* t = trips[tindex];
                revenue += tk->seats * t->fare;
            }
        }
        if (strcmp(tk->status, "cancelled") == 0) {
            totalCancelled += tk->seats;
        }
    }
    printf("=== Statistical report ===\n");
    printf("Total number of seats booked: %d\n", totalBooked);
    printf("Total number of seats paid for: %d\n", totalPaid);
    printf("Total number of cancelled seats: %d\n", totalCancelled);
    printf("Total revenue (Only pay for ticket): %.2f\n", revenue);
}

void listTickets() {
    if (ticketCount == 0) {
        printf("There are no ticket yet.\n");
        return;
    }
    printf("Ticket List:\n");
    printf("TID\tTrip\tPassenger\tSeats\tPaid\tStatus\n");
    for (int i = 0; i < ticketCount; ++i) {
        Ticket* tk = tickets[i];
        if (tk) {
            printf("%d\t%d\t%s\t%d\t%s\t%s\n",
                tk->ticketId, tk->tripId, tk->passengerName, tk->seats,
                tk->paymentStatus ? "Y" : "N", tk->status);
        }
    }
}

void cleanup() {
    for (int i = 0; i < tripCount; ++i) if (trips[i]) free(trips[i]);
    for (int i = 0; i < ticketCount; ++i) if (tickets[i]) free(tickets[i]);
}

void preloadSampleData() {
    addTrip(101, "Hanoi", "2025-12-01 08:00", 40, 150000.0);
    addTrip(102, "Saigon", "2025-12-02 07:30", 50, 350000.0);
    bookTicket(1001, 101, "Nguyen Van A", 2);
    bookTicket(1002, 101, "Tran Thi B", 1);
    bookTicket(1003, 102, "Le Van C", 3);
    payTicket(1001);
}

void showMenu() {
    printf("\n======= Bus ticket Management System =======\n");
    printf("1. Add new trip\n");
    printf("2. Update trip\n");
    printf("3. List trips\n");
    printf("4. Book ticket \n");
    printf("5. Check ticket by ticketId\n");
    printf("6. List bus tickets\n");
    printf("7. Ticket payment\n");
    printf("8. Lock ticket\n");
    printf("9. Cancel ticket\n");
    printf("10. Statistics & Revenue Reports\n");
    printf("0. Exit\n");
    printf("Select function: ");
}

int main() {
    char buf[STR_LEN];
    int choice;
    preloadSampleData();
    while (1) {
        showMenu();
        read_line(buf, STR_LEN);
        choice = atoi(buf);
    
        switch (choice) {
    
            case 0:
                printf("Thoát chuong trình. T?m bi?t!\n");
                cleanup();
                return 0;
    
            case 1: {
                printf("Add new trip\n");
    
                printf("Enter trip ID: "); read_line(buf, STR_LEN); 
                int tripId = atoi(buf);
    
                printf("Enter destination: "); 
                char dest[50]; read_line(dest, 50); trim(dest);
    
                printf("Enter departure time: "); 
                char timeS[50]; read_line(timeS, 50); trim(timeS);
    
                printf("Enter total seats: "); 
                read_line(buf, STR_LEN); int seats = atoi(buf);
    
                printf("Enter fare: "); 
                read_line(buf, STR_LEN); double fare = atof(buf);
    
                int r = addTrip(tripId, dest, timeS, seats, fare);
                if (r == 0) printf("Add trip successfully!\n");
                else printf("Error: error code %d\n", r);
                break;
            }
    
            case 2: {
                printf("Update trip\n");
    
                printf("Enter trip ID: "); read_line(buf, STR_LEN); 
                int tripId = atoi(buf);
    
                if (findTripIndexById(tripId) == -1) {
                    printf("Trip not found!\n");
                    break;
                }
    
                printf("Enter new destination: "); 
                char dest[50]; read_line(dest, 50); trim(dest);
    
                printf("Enter new departure time: "); 
                char timeS[50]; read_line(timeS, 50); trim(timeS);
    
                printf("Enter new total seats: "); 
                read_line(buf, STR_LEN); int seats = atoi(buf);
    
                printf("Enter new fare: "); 
                read_line(buf, STR_LEN); double fare = atof(buf);
    
                int r = updateTrip(tripId, dest, timeS, seats, fare);
                if (r == 0) printf("Update trip successfully!\n");
                else printf("Error: error code %d\n", r);
                break;
            }
    
            case 3:
                listTrips();
                break;
    
            case 4: {
                printf("Book ticket\n");
    
                printf("Enter ticket ID: "); read_line(buf, STR_LEN); 
                int tid = atoi(buf);
    
                printf("Enter trip ID: "); read_line(buf, STR_LEN); 
                int tripId = atoi(buf);
    
                printf("Enter passenger name: "); 
                char name[50]; read_line(name, 50); trim(name);
    
                printf("Enter seats: "); 
                read_line(buf, STR_LEN); int seats = atoi(buf);
    
                int r = bookTicket(tid, tripId, name, seats);
                if (r == 0) printf("Book ticket successfully!\n");
                else printf("Error: error code %d\n", r);
                break;
            }
    
            case 5:
                printf("Enter ticket ID: "); 
                read_line(buf, STR_LEN);
                checkTicket(atoi(buf));
                break;
    
            case 6:
                listTickets();
                break;
    
            case 7: {
                printf("Ticket payment\nEnter ticket ID: "); 
                read_line(buf, STR_LEN);
                int r = payTicket(atoi(buf));
                printf(r == 0 ? "Ticket payment successfully!\n" : "Ticket payment failed!\n");
                break;
            }
    
            case 8: {
                printf("Lock ticket\nEnter ticket ID: "); 
                read_line(buf, STR_LEN);
                int r = lockTicket(atoi(buf));
                printf(r == 0 ? "Lock ticket successfully!\n" : "Lock ticket failed!\n");
                break;
            }
    
            case 9: {
                printf("Cancel ticket\n");
                read_line(buf, STR_LEN);
                int r = cancelTicket(atoi(buf));
                printf(r == 0 ? "H?y vé thành công!\n" : "H?y vé th?t b?i!\n");
                break;
            }
    
            case 10:
                reportStatistics();
                break;
    
            default:
                printf("L?a ch?n không h?p l?!\n");
        }
    }
    cleanup();
    return 0;
}
