#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAXTICKETS 100
#define MAX_TRIPS 100

struct Location {
    char name[50];       // ten dia diem
    char address[100];   // dia chi cua dia diem
};
struct Trip {
    char tripId[20]; // Ma chuyen xe
    struct Location departure; // Diem khoi hanh
    struct Location destination; // Diem den
    char date[20]; // Ngay gio chuyen xe
    int totalSeats; // Tong so ghe
    int bookedSeats; // So ghe da dat
};
// Danh sach Trip
struct Trip trips[MAX_TRIPS] = {
    {
        "TX001",
        {"Ha Noi", "So 1 Trang Tien, Hoan Kiem, Ha Noi"},
        {"Hai Phong", "So 2 Le Hong Phong, Hai Phong"},
        "2024-07-01 08:00",
        40,
        10
    },
    {
        "TX002",
        {"Da Nang", "So 3 Nguyen Van Linh, Da Nang"},
        {"Hue", "So 4 Hung Vuong, Hue"},
        "2024-07-02 09:30",
        45,
        20
    },
    {
        "TX003",
        {"TP HCM", "So 5 Ly Tu Trong, Quan 1, TP HCM"},
        {"Nha Trang", "So 6 Tran Phu, Nha Trang"},
        "2024-07-03 07:45",
        50,
        15
    },
    {
        "TX004",
        {"Can Tho", "So 7 Cach Mang Thang 8, Can Tho"},
        {"Bac Lieu", "So 8 Tran Huynh, Bac Lieu"},
        "2024-07-04 10:00",
        35,
        12
    },
    {
        "TX005",
        {"Vinh", "So 9 Nguyen Du, TP Vinh"},
        {"Thanh Hoa", "So 10 Hac Thanh, Thanh Hoa"},
        "2024-07-05 13:15",
        30,
        5
    }
};

struct Passenger {
    char name[50]; // Ten hanh khach
    char phone[20]; // So dien thoai hanh khach
};
struct Ticket {
    char ticketId[20]; // Ma ve (Sinh ngau nhien hoac tu dong)
    char tripId[20]; // Ma chuyen xe
    struct Passenger passenger; // Thong tin hanh khach
    int seatNumber; // so ghe da dat
    double price; // Gia ve
    int paymentStatus; // Trang thai thanh toan (0: chua thanh toan,1: da thanh toan, -1: vo hieu hoa)
    char date[32]; // Ngay gio dat ve
};
// danh sach ticket 
struct Ticket tickets[MAXTICKETS] = {
    {
        "V001",
        "TX001",
        {"Nguyen Van A", "0912345678"},
        1,
        250000.0,
        1,
        "2024-06-28 07:30"
    },
    {
        "V002",
        "TX002",
        {"Tran Thi B", "0922233344"},
        5,
        300000.0,
        0,
        "2024-07-01 09:00"
    },
    {
        "V003",
        "TX003",
        {"Le Van C", "0933456789"},
        10,
        350000.0,
        1,
        "2024-07-02 09:30"
    },
    {
        "V004",
        "TX004",
        {"Pham Thi D", "0944556677"},
        2,
        200000.0,
        1,
        "2024-07-03 07:45"
    },
    {
        "V005",
        "TX005",
        {"Hoang Van E", "0955667788"},
        8,
        270000.0,
        0,
        "2024-07-04 10:00"
    }
};
// Gan gia tri
int ticketCount = 5;
int tripCount = 5;

// khai bao ham
void clearInputBuffer();
void inputString(char *prompt, char *str, int size);
int isTripIdUnique(const char *tripId);
int isEmptyString(const char *str);
void showMenu();
void addTrip();
void updateTripInfo();
void bookTicket();
void checkTicketStatus(struct Ticket tickets[], int ticketCount, struct Trip trips[], int tripCount);
int isValidPhone(const char* phone);
int findTripIndexById(const char* tripId);
int isSeatBooked(const char* tripId, int seatNumber, struct Ticket* tickets, int ticketCount);
void listOfTrips();
void payTickets();
void manageTicketStatus();

int main() {
    int choice;
    while(1){
        showMenu();
        printf("Moi ban nhap lua chon: ");
        char choiceBuf[32];
        if (fgets(choiceBuf, sizeof(choiceBuf), stdin) == NULL) {
            printf("Loi! Vui long thu lai\n");
            clearInputBuffer();
            continue;
        }
        choice = atoi(choiceBuf);
        switch(choice){
            case 1:
                addTrip();
                break;
            case 2:
                updateTripInfo();
                break;
            case 3:
                bookTicket();
                break;
            case 4:
                checkTicketStatus(tickets, ticketCount, trips, tripCount);
                break;
            case 5:
                listOfTrips();
                break;
            case 6:
                payTickets();
                break;
            case 7:
                manageTicketStatus();
                break;
            case 8:
                printf("Chuc nang thong ke chua duoc implement.\n");
                break;
            case 9:
                printf("Thoat chuong trinh...!\n");
                return 0;
            default :
                printf("Loi! Moi ban nhap lai\n");
        }
    }
    return 0;
}
// Ham xoa buffer 
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Giup nhap chuoi voi loai bo dau '\n' cuoi
void inputString(char *prompt, char *str, int size) {
    while(1) {
        if (prompt && strlen(prompt) > 0) printf("%s", prompt);
        if (fgets(str, size, stdin) == NULL) {
            str[0] = '\0';
        } else {
            str[strcspn(str, "\n")] = '\0'; // xoa \n cuoi
        }
        if(strlen(str) == 0) {
            printf("Khong duoc bo trong.\n");
            continue;
        }
        break;
    }
}

// Ham kiem tra su duy nhat cua TripID
int isTripIdUnique(const char *tripId) {
    for (int i = 0; i < tripCount; i++) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            return 0; // Khong duy nhat
        }
    }
    return 1; // Duy nhat
}

// Ham kiem tra chuoi rong (bo trong)
int isEmptyString(const char *str) {
    return (str == NULL) || (strlen(str) == 0);
}

void showMenu() {
    const char *menuLines[] = {
        "========== QUAN LY VE XE KHACH ==========",
        "| 1. Them chuyen xe moi                  |",
        "| 2. Cap nhat thong tin chuyen xe        |",
        "| 3. Dat ve                              |",
        "| 4. Kiem tra tinh trang ve              |",
        "| 5. Liet ke chuyen xe                   |",
        "| 6. Thanh toan ve                       |",
        "| 7. Quan ly trang thai ve (Khoa, Huy)   |",
        "| 8. Bao cao thong ke va doanh thu       |",
        "| 9. Thoat                               |",
        "=========================================="
    };
    int n = sizeof(menuLines) / sizeof(menuLines[0]);
    for (int i = 0; i < n; i++) {
        printf("%s\n", menuLines[i]);
    }
}

// Ham them chuyen xe moi
void addTrip() {
    if (tripCount >= MAX_TRIPS) {
        printf("Them chuyen xe that bai: Danh sach chuyen xe da day.\n");
        return;
    }

    struct Trip newTrip;
    char temp[100];
    int valid = 1; // Gia su du lieu hop le ban dau

    // Nhap va kiem tra ma chuyen xe (TripID): khong rong, khong trung
    inputString("Nhap ma chuyen xe (duy nhat): ", temp, sizeof(temp));
    if (isEmptyString(temp)) {
        printf("Them chuyen xe that bai: Ma chuyen xe khong duoc de trong.\n");
        valid = 0;
    } else if (!isTripIdUnique(temp)) {
        printf("Them chuyen xe that bai: Ma chuyen xe da ton tai.\n");
        valid = 0;
    } else {
        strncpy(newTrip.tripId, temp, sizeof(newTrip.tripId));
        newTrip.tripId[sizeof(newTrip.tripId)-1] = '\0';
    }

    // Nhap va kiem tra ten diem khoi hanh (khong rong)
    if (valid) {
        inputString("Nhap ten diem khoi hanh: ", newTrip.departure.name, sizeof(newTrip.departure.name));
        if (isEmptyString(newTrip.departure.name)) {
            printf("Them chuyen xe that bai: Ten diem khoi hanh khong duoc de trong.\n");
            valid = 0;
        }
    }

    // Nhap dia chi diem khoi hanh (khong kiem tra rong)
    if (valid) 
        inputString("Nhap dia chi diem khoi hanh: ", newTrip.departure.address, sizeof(newTrip.departure.address));

    // Nhap va kiem tra ten diem den (khong rong)
    if (valid) {
        inputString("Nhap ten diem den: ", newTrip.destination.name, sizeof(newTrip.destination.name));
        if (isEmptyString(newTrip.destination.name)) {
            printf("Them chuyen xe that bai: Ten diem den khong duoc de trong.\n");
            valid = 0;
        }
    }

    // Nhap dia chi diem den (khong kiem tra rong)
    if (valid) 
        inputString("Nhap dia chi diem den: ", newTrip.destination.address, sizeof(newTrip.destination.address));

    // Nhap ngay gio chuyen xe (khong rong)
    if (valid) {
        inputString("Nhap ngay gio chuyen xe: ", newTrip.date, sizeof(newTrip.date));
        if (isEmptyString(newTrip.date)) {
            printf("Them chuyen xe that bai: Ngay gio chuyen xe khong duoc de trong.\n");
            valid = 0;
        }
    }

    // Nhap tong so ghe (kiem tra > 0)
    if (valid) {
        inputString("Nhap tong so ghe: ", temp, sizeof(temp));
        newTrip.totalSeats = atoi(temp);
        if (isEmptyString(temp)) {
            printf("Them chuyen xe that bai: Tong so ghe khong duoc de trong.\n");
            valid = 0;
        } else if (newTrip.totalSeats <= 0) {
            printf("Them chuyen xe that bai: Tong so ghe phai lon hon 0.\n");
            valid = 0;
        }
    }

    if (valid) {
        newTrip.bookedSeats = 0;
        trips[tripCount++] = newTrip;
        printf("Them chuyen xe thanh cong!\n");
    } else {
        printf("Them chuyen xe that bai: Du lieu khong hop le.\n");
    }
}

void updateTripInfo() {
    char tripId[20];
    inputString("Nhap ma chuyen xe can cap nhat: ", tripId, sizeof(tripId));

    // Validate tripId: Khong de trong
    if (isEmptyString(tripId)) {
        printf("Ma chuyen xe khong duoc de trong.\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < tripCount; ++i) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            found = 1;

            char newDepartureName[100];
            char newDepartureAddr[100];
            char newDestinationName[100];
            char newDestinationAddr[100];
            char newDate[30];
            char temp[32];
            int newTotalSeats;

            // Nhap va kiem tra ten diem khoi hanh moi (khong de trong)
            inputString("Nhap ten diem khoi hanh moi: ", newDepartureName, sizeof(newDepartureName));
            if (isEmptyString(newDepartureName)) {
                printf("Ten diem khoi hanh khong duoc de trong.\n");
                return;
            }
            inputString("Nhap dia chi diem khoi hanh moi: ", newDepartureAddr, sizeof(newDepartureAddr));

            // Nhap va kiem tra ten diem den moi (khong de trong)
            inputString("Nhap ten diem den moi: ", newDestinationName, sizeof(newDestinationName));
            if (isEmptyString(newDestinationName)) {
                printf("Ten diem den khong duoc de trong.\n");
                return;
            }
            inputString("Nhap dia chi diem den moi: ", newDestinationAddr, sizeof(newDestinationAddr));

            // Nhap ngay gio chuyen xe moi (khong de trong)
            inputString("Nhap ngay gio moi cho chuyen xe: ", newDate, sizeof(newDate));
            if (isEmptyString(newDate)) {
                printf("Ngay gio chuyen xe khong duoc de trong.\n");
                return;
            }

            // Nhap tong so ghe moi (> 0 va >= bookedSeats)
            while (1) {
                inputString("Nhap tong so ghe moi: ", temp, sizeof(temp));
                if (isEmptyString(temp)) {
                    printf("Tong so ghe khong duoc de trong.\n");
                    continue;
                }
                newTotalSeats = atoi(temp);
                if (newTotalSeats < trips[i].bookedSeats) {
                    printf("Tong so ghe khong duoc nho hon so ghe da dat (%d).\n", trips[i].bookedSeats);
                } else if (newTotalSeats <= 0) {
                    printf("Tong so ghe phai lon hon 0.\n");
                } else {
                    break;
                }
            }

            // Cap nhat du lieu chuyen xe
            strncpy(trips[i].departure.name, newDepartureName, sizeof(trips[i].departure.name));
            trips[i].departure.name[sizeof(trips[i].departure.name)-1] = '\0';
            strncpy(trips[i].departure.address, newDepartureAddr, sizeof(trips[i].departure.address));
            trips[i].departure.address[sizeof(trips[i].departure.address)-1] = '\0';
            strncpy(trips[i].destination.name, newDestinationName, sizeof(trips[i].destination.name));
            trips[i].destination.name[sizeof(trips[i].destination.name)-1] = '\0';
            strncpy(trips[i].destination.address, newDestinationAddr, sizeof(trips[i].destination.address));
            trips[i].destination.address[sizeof(trips[i].destination.address)-1] = '\0';
            strncpy(trips[i].date, newDate, sizeof(trips[i].date));
            trips[i].date[sizeof(trips[i].date)-1] = '\0';
            trips[i].totalSeats = newTotalSeats;

            printf("Cap nhat chuyen xe thanh cong\n");
            return;
        }
    }
    // Neu khong tim thay chuyen xe
    if (!found) {
        printf("Khong tim thay chuyen xe\n");
    }
}

// Ham kiem tra so dien thoai (chi cho phep so va do dai tu 8-15)
int isValidPhone(const char* phone) {
    int len = strlen(phone);
    if (len < 8 || len > 15)
        return 0;
    for (int i = 0; i < len; ++i)
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    return 1;
}

// Kiem tra ma chuyen xe ton tai (theo tripId) - tra ve index, khong thi -1
int findTripIndexById(const char* tripId) {
    for (int i = 0; i < tripCount; ++i)
        if (strcmp(trips[i].tripId, tripId) == 0)
            return i;
    return -1;
}

// Kiem tra ghe da dat cho tripId, seatNumber
int isSeatBooked(const char* tripId, int seatNumber, struct Ticket* tickets, int ticketCount) {
    for (int i = 0; i < ticketCount; ++i)
        if (strcmp(tickets[i].tripId, tripId) == 0 && tickets[i].seatNumber == seatNumber)
            return 1;
    return 0;
}

// Ham dat ve (Book Ticket)
void bookTicket() { // Chuc nang dat ve
    extern struct Ticket tickets[];
    extern int ticketCount;

    if (tripCount == 0) {
        printf("Dat ve that bai: Khong co chuyen xe nao!\n");
        return;
    }

    printf("----- Danh sach cac chuyen xe -----\n");
    for (int i = 0; i < tripCount; ++i) {
        printf("%d. Ma chuyen xe: %s | Khoi hanh: %s | Den: %s | Ghe trong: %d\n",
               i + 1, trips[i].tripId,
               trips[i].departure.name, trips[i].destination.name,
               trips[i].totalSeats - trips[i].bookedSeats);
    }

    char inputTripId[32];
    int tripIdx = -1;

    // Vong lap nhap ma chuyen xe hop le
    while (1) {
        inputString("Nhap ma chuyen xe muon dat ve: ", inputTripId, sizeof(inputTripId));
        if (isEmptyString(inputTripId)) {
            printf("Ma chuyen xe khong duoc de trong!\n");
            printf("Dat ve that bai!\n");
            return;
        }
        tripIdx = findTripIndexById(inputTripId);
        if (tripIdx == -1) {
            printf("Khong tim thay chuyen xe.\n");
            printf("Dat ve that bai!\n");
            return;
        }
        if (trips[tripIdx].bookedSeats >= trips[tripIdx].totalSeats) {
            printf("Chuyen xe da het ghe!\n");
            printf("Dat ve that bai!\n");
            return;
        }
        break;
    }

    struct Ticket newTicket;
    memset(&newTicket, 0, sizeof(struct Ticket));
    strncpy(newTicket.tripId, trips[tripIdx].tripId, sizeof(newTicket.tripId) - 1);
    newTicket.tripId[sizeof(newTicket.tripId) - 1] = '\0';

    // Vong lap nhap thong tin ve den khi hop le
    while (1) {
        inputString("Nhap ten hanh khach: ", newTicket.passenger.name, sizeof(newTicket.passenger.name));
        if (isEmptyString(newTicket.passenger.name)) {
            printf("Ten hanh khach khong duoc de trong!\n");
            continue;
        }
        inputString("Nhap so dien thoai hanh khach: ", newTicket.passenger.phone, sizeof(newTicket.passenger.phone));
        if (isEmptyString(newTicket.passenger.phone)) {
            printf("So dien thoai khong duoc de trong!\n");
            continue;
        }
        if (!isValidPhone(newTicket.passenger.phone)) {
            printf("So dien thoai khong dung dinh dang hoac khong hop le!\n");
            continue;
        }
		break;  
	} 
	while(1){
        char tempSeat[16];
        int seatNumber;
        printf("Nhap so ghe (1-%d): ", trips[tripIdx].totalSeats);
        inputString("", tempSeat, sizeof(tempSeat));
        seatNumber = atoi(tempSeat);

        if (seatNumber < 1 || seatNumber > trips[tripIdx].totalSeats) {
            printf("So ghe phai > 0 va <= tong so ghe!\n");
            continue;
        }
        if (isSeatBooked(trips[tripIdx].tripId, seatNumber, tickets, ticketCount)) {
            printf("So ghe nay da duoc dat trong chuyen xe nay!\n");
            continue;
        }
        break;
	}
	while(1){
		int seatNumber; 
        newTicket.seatNumber = seatNumber;

        newTicket.paymentStatus = 0;

        time_t now = time(NULL);
        struct tm *tm_info = localtime(&now);
        strftime(newTicket.date, sizeof(newTicket.date), "%Y-%m-%d %H:%M:%S", tm_info);

        char tempPrice[32];
        inputString("Nhap gia ve: ", tempPrice, sizeof(tempPrice));
        newTicket.price = atof(tempPrice);
        if (newTicket.price <= 0) {
            printf("Gia ve phai lon hon 0!\n");
            continue;
        }
        break;
    }

    snprintf(newTicket.ticketId, sizeof(newTicket.ticketId), "V%03d", ticketCount + 1);
    tickets[ticketCount++] = newTicket;
    trips[tripIdx].bookedSeats++;

    printf("Dat ve thanh cong\n");

    printf("----- Danh sach ve da dat -----\n");
    for (int i = 0; i < ticketCount; ++i) {
        printf("Ma ve: %s | Ma chuyen xe: %s | Ten khach: %s | SDT: %s | Ghe: %d | Gia: %.2f | Ngay dat: %s\n",
            tickets[i].ticketId, tickets[i].tripId, tickets[i].passenger.name,
            tickets[i].passenger.phone, tickets[i].seatNumber, tickets[i].price, tickets[i].date);
    }
}

void listOfTrips() {
    if (tripCount == 0) {
        printf("Khong co chuyen xe de hien thi!\n");
        return;
    }

    char temp[32];
    int pageSize = 10;
    int pageNumber = 1;

    printf("Nhap so luong dong moi trang (mac dinh 10): ");
    if (fgets(temp, sizeof(temp), stdin) != NULL) {
        int val = atoi(temp);
        if (val > 0) pageSize = val;
    }

    printf("Nhap so trang muon xem (mac dinh 1): ");
    if (fgets(temp, sizeof(temp), stdin) != NULL) {
        int val = atoi(temp);
        if (val > 0) pageNumber = val;
    }

    int totalPages = (tripCount + pageSize - 1) / pageSize;

    if (pageNumber > totalPages) {
        printf("So trang khong hop le! Tong so trang: %d\n", totalPages);
        return;
    }

    printf("\n===== DANH SACH CHUYEN XE (Trang %d/%d) =====\n", pageNumber, totalPages);

    int startIndex = (pageNumber - 1) * pageSize;
    int endIndex = startIndex + pageSize;
    if (endIndex > tripCount) endIndex = tripCount;

    for (int i = startIndex; i < endIndex; i++) {
        printf("\nChuyen %d:\n", i + 1);
        printf("  Ma chuyen xe   : %s\n", trips[i].tripId);
        printf("  Diem khoi hanh : %s (%s)\n",
               trips[i].departure.name, trips[i].departure.address);
        printf("  Diem den       : %s (%s)\n",
               trips[i].destination.name, trips[i].destination.address);
        printf("  Ngay gio       : %s\n", trips[i].date);
        printf("  Ghe da dat     : %d/%d\n",
               trips[i].bookedSeats, trips[i].totalSeats);
    }

    printf("\n===== Dieu huong trang =====\n");
    if (pageNumber > 1) printf("Prev (nhap: %d)\n", pageNumber - 1);
    if (pageNumber < totalPages) printf("Next (nhap: %d)\n", pageNumber + 1);
}

// Thanh toan ve
void payTickets(){
    if (ticketCount == 0) {
        printf("Khong co ve de thanh toan \n");
        return;
    }

    char ticketId[32];
    inputString("Nhap ma ve can thanh toan: ", ticketId, sizeof(ticketId));
    if (isEmptyString(ticketId)) {
        printf("Ma ve khong duoc de trong!\n");
        return;
    }

    int found = 0;
    for (int i = 0; i < ticketCount; ++i) {
        if (strcmp(tickets[i].ticketId, ticketId) == 0) {
            found = 1;

            if (tickets[i].paymentStatus == -1) {
                printf("Ve khong the thanh toan vi da bi vo hieu hoa!\n");
                return;
            }
            if (tickets[i].paymentStatus == 1) {
                printf("Ve da thanh toan truoc do!\n");
                return;
            }

            // Thuc hien thanh toan
            tickets[i].paymentStatus = 1;
            printf("Thanh toan thanh cong cho ve %s!\n", tickets[i].ticketId);
            return;
        }
    }

    if (!found) {
        printf("Khong tim thay ve!\n");
    }
}

// Quan ly trang thai ve: khoa/huy (dat -1 la vo hieu hoa)
void manageTicketStatus() {
    if (ticketCount == 0) {
        printf("Khong co ve de quan ly.\n");
        return;
    }
    char ticketId[32];
    inputString("Nhap ma ve can quan ly: ", ticketId, sizeof(ticketId));
    if (isEmptyString(ticketId)) {
        printf("Ma ve khong duoc de trong!\n");
        return;
    }

    int idx = -1;
    for (int i = 0; i < ticketCount; ++i) {
        if (strcmp(tickets[i].ticketId, ticketId) == 0) {
            idx = i;
            break;
        }
    }
    if (idx == -1) {
        printf("Khong tim thay ve!\n");
        return;
    }

    printf("1. Khoa/Vo hieu hoa ve\n");
    printf("2. Huy khoa (dat lai trang thai chua thanh toan)\n");
    char choiceBuf[8];
    inputString("Lua chon: ", choiceBuf, sizeof(choiceBuf));
    int c = atoi(choiceBuf);
    if (c == 1) {
        tickets[idx].paymentStatus = -1;
        // giam bookedSeats tren chuyen do neu can (tu quyet dinh business)
        // trips[...]-- not changed here to keep seat record (optional)
        printf("Ve %s da duoc vo hieu hoa.\n", ticketId);
    } else if (c == 2) {
        if (tickets[idx].paymentStatus == -1) {
            tickets[idx].paymentStatus = 0;
            printf("Ve %s da duoc mo khoa (chua thanh toan).\n", ticketId);
        } else {
            printf("Ve khong o trang thai vo hieu hoa.\n");
        }
    } else {
        printf("Lua chon khong hop le.\n");
    }
}
