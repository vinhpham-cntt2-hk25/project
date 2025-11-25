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
    // right trim
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
    if (tripCount >= MAX_TRIPS) return -2; // full
    if (totalSeats <= 0) return -3; // invalid seats
    if (findTripIndexById(tripId) != -1) return -1; // duplicate id

    Trip* t = (Trip*) malloc(sizeof(Trip));
    if (!t) return -4; // mem error
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
    if (idx == -1) return -1; // not found

    Trip* t = trips[idx];
    if (totalSeats < t->bookedSeats) return -2; // cannot set total less than already booked

    strncpy(t->destination, destination, sizeof(t->destination)-1); t->destination[sizeof(t->destination)-1] = '\0';
    strncpy(t->time, time, sizeof(t->time)-1); t->time[sizeof(t->time)-1] = '\0';
    t->totalSeats = totalSeats;
    t->fare = fare;
    return 0;
}

void listTrips() {
    if (tripCount == 0) {
        printf("Chua có chuy?n nào.\n");
        return;
    }
    printf("Danh sách chuy?n:\n");
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
    if (ticketCount >= MAX_TICKETS) return -4; // tickets full
    if (seats <= 0) return -3;

    if (findTicketIndexById(ticketId) != -1) return -1; // ticketId duplicate

    int tidx = findTripIndexById(tripId);
    if (tidx == -1) return -2; // trip not found

    Trip* t = trips[tidx];
    if (t->bookedSeats + seats > t->totalSeats) return -5; // not enough seats

    Ticket* tk = (Ticket*) malloc(sizeof(Ticket));
    if (!tk) return -6; // mem error
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
    if (idx == -1) return -1; // not found

    Ticket* tk = tickets[idx];
    if (strcmp(tk->status, "active") != 0) return -2; // not active
    if (tk->paymentStatus == 1) return -3; // already paid

    tk->paymentStatus = 1;
    return 0;
}

int lockTicket(int ticketId) {
    int idx = findTicketIndexById(ticketId);
    if (idx == -1) return -1;
    Ticket* tk = tickets[idx];
    if (strcmp(tk->status, "active") != 0) return -2; // only lock active
    strncpy(tk->status, "locked", sizeof(tk->status)-1);
    tk->status[sizeof(tk->status)-1] = '\0';
    return 0;
}

int cancelTicket(int ticketId) {
    int idx = findTicketIndexById(ticketId);
    if (idx == -1) return -1;
    Ticket* tk = tickets[idx];
    if (strcmp(tk->status, "active") != 0) return -2; // only cancel active
    if (tk->paymentStatus == 1) return -3; // cannot cancel paid ticket per SRS
    // free seats from trip
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
    printf("=== Báo cáo th?ng kê ===\n");
    printf("T?ng s? gh? d?t: %d\n", totalBooked);
    printf("T?ng s? gh? dã thanh toán: %d\n", totalPaid);
    printf("T?ng s? gh? dã h?y: %d\n", totalCancelled);
    printf("T?ng doanh thu (ch? tính vé dã thanh toán): %.2f\n", revenue);
}

void listTickets() {
    if (ticketCount == 0) {
        printf("Chua có vé nào.\n");
        return;
    }
    printf("Danh sách vé:\n");
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
    printf("\n======= QU?N LÝ VÉ XE KHÁCH =======\n");
    printf("1. Thêm chuy?n xe m?i\n");
    printf("2. C?p nh?t chuy?n xe\n");
    printf("3. Li?t kê chuy?n xe\n");
    printf("4. Ð?t vé\n");
    printf("5. Ki?m tra vé theo ticketId\n");
    printf("6. Li?t kê vé\n");
    printf("7. Thanh toán vé\n");
    printf("8. Khóa vé\n");
    printf("9. H?y vé\n");
    printf("10. Báo cáo th?ng kê & doanh thu\n");
    printf("0. Thoát\n");
    printf("Ch?n ch?c nang: ");
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
            printf("Thoát chương trình. Tạm biệt!\n");
            cleanup();
            return 0;

        case 1: {
            printf("Thêm chuyến xe mới\n");

            printf("Nhập mã chuyến: "); read_line(buf, STR_LEN); 
            int tripId = atoi(buf);

            printf("Nhập điểm đến: "); 
            char dest[50]; read_line(dest, 50); trim(dest);

            printf("Nhập thời gian khởi hành: "); 
            char timeS[50]; read_line(timeS, 50); trim(timeS);

            printf("Nhập tổng số ghế: "); 
            read_line(buf, STR_LEN); int seats = atoi(buf);

            printf("Nhập giá vé: "); 
            read_line(buf, STR_LEN); double fare = atof(buf);

            int r = addTrip(tripId, dest, timeS, seats, fare);
            if (r == 0) printf("Thêm chuyến thành công!\n");
            else printf("Lỗi: mã lỗi %d\n", r);
            break;
        }

        case 2: {
            printf("Cập nhật chuyến xe\n");

            printf("Nhập mã chuyến: "); read_line(buf, STR_LEN); 
            int tripId = atoi(buf);

            if (findTripIndexById(tripId) == -1) {
                printf("Không tìm thấy chuyến xe!\n");
                break;
            }

            printf("Nhập điểm đến mới: "); 
            char dest[50]; read_line(dest, 50); trim(dest);

            printf("Nhập thời gian mới: "); 
            char timeS[50]; read_line(timeS, 50); trim(timeS);

            printf("Nhập tổng ghế mới: "); 
            read_line(buf, STR_LEN); int seats = atoi(buf);

            printf("Nhập giá vé mới: "); 
            read_line(buf, STR_LEN); double fare = atof(buf);

            int r = updateTrip(tripId, dest, timeS, seats, fare);
            if (r == 0) printf("Cập nhật thành công!\n");
            else printf("Lỗi: mã lỗi %d\n", r);
            break;
        }

        case 3:
            listTrips();
            break;

        case 4: {
            printf("Đặt vé\n");

            printf("Nhập mã vé: "); read_line(buf, STR_LEN); 
            int tid = atoi(buf);

            printf("Nhập mã chuyến: "); read_line(buf, STR_LEN); 
            int tripId = atoi(buf);

            printf("Nhập tên hành khách: "); 
            char name[50]; read_line(name, 50); trim(name);

            printf("Nhập số ghế đặt: "); 
            read_line(buf, STR_LEN); int seats = atoi(buf);

            int r = bookTicket(tid, tripId, name, seats);
            if (r == 0) printf("Đặt vé thành công!\n");
            else printf("Lỗi: mã lỗi %d\n", r);
            break;
        }

        case 5:
            printf("Nhập mã vé: "); 
            read_line(buf, STR_LEN);
            checkTicket(atoi(buf));
            break;

        case 6:
            listTickets();
            break;

        case 7: {
            printf("Thanh toán vé\nNhập mã vé: "); 
            read_line(buf, STR_LEN);
            int r = payTicket(atoi(buf));
            printf(r == 0 ? "Thanh toán thành công!\n" : "Thanh toán thất bại!\n");
            break;
        }

        case 8: {
            printf("Khóa vé\nNhập mã vé: "); 
            read_line(buf, STR_LEN);
            int r = lockTicket(atoi(buf));
            printf(r == 0 ? "Khóa vé thành công!\n" : "Khóa vé thất bại!\n");
            break;
        }

        case 9: {
            printf("Hủy vé\nNhập mã vé: "); 
            read_line(buf, STR_LEN);
            int r = cancelTicket(atoi(buf));
            printf(r == 0 ? "Hủy vé thành công!\n" : "Hủy vé thất bại!\n");
            break;
        }

        case 10:
            reportStatistics();
            break;

        default:
            printf("Lựa chọn không hợp lệ!\n");
    }
}
    return 0;
}

