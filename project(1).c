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
    char tripId[20]; // ma chuyen xe
    struct Location departure; // diem khoi hanh
    struct Location destination; // diem den
    char date[20]; // ngay gio chuyen xe
    int totalSeats; // tong so ghe
    int bookedSeats; // so ghe da dat
};
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
    char name[50]; // ten hanh khach
    char phone[20]; // so dien thoai hanh khach
};
struct Ticket {
    char ticketId[20]; // ma ve (sinh ngau nhien hoac tu dong)
    char tripId[20]; // ma chuyen xe
    struct Passenger passenger; // thong tin hanh khach
    int seatNumber; // so ghe da dat
    double price; // gia ve
    int paymentStatus; // trang thai thanh toan (0: chua thanh toan, 1: da thanh toan)
    char date[32]; // ngay gio dat ve
    int status; // 0:chua thanh toan, 1:huy, 2:khoa 
};
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
int ticketCount = 5; // bien dem so luong ve
int tripCount = 5; // bien dem so luong chuyen xe

// khai bao prototype ham
void inputString(char *prompt, char *str, int size); // nhap xau ky tu
int isTripIdUnique(const char *tripId); // kiem tra ma chuyen xe co duy nhat khong
int isEmptyString(const char *str); // kiem tra xau co trong khong
void showMenu(); // hien thi menu
void addTrip(); // them chuyen xe
void updateTripInfo(); // cap nhat thong tin chuyen xe
void bookTicket(); // dat ve
int isValidPhone(const char* phone); // kiem tra so dien thoai hop le khong
int findTripIndexById(const char* tripId); // tim chi so chuyen xe theo ma
int isSeatBooked(const char* tripId, int seatNumber, struct Ticket* tickets, int ticketCount); // kiem tra ghe da dat
void checkTicketStatus(struct Ticket tickets[], int ticketCount, struct Trip trips[], int tripCount); // kiem tra tinh trang ve
void listTrips(); // liet ke chuyen xe
void payTicket(); // thanh toan ve
int main() { 
    int choice; // bien lua chon
    while (1) { // vong lap menu
        showMenu(); // goi ham hien thi menu
        printf("Moi ban nhap lua chon: "); // thong bao nhap lua chon
        scanf("%d", &choice);
        getchar(); // doc ky tu enter con lai
        switch(choice){
            case 1:
                addTrip(); // goi ham them chuyen xe
                break;
            case 2:
                updateTripInfo(); // goi ham cap nhat chuyen xe
                break;
            case 3:
                bookTicket(); // goi ham dat ve
                break;
            case 4:
                checkTicketStatus(tickets, ticketCount, trips, tripCount); // kiem tra ve
                break;
            case 5:
                listTrips(); // liet ke chuyen xe
                break;
            case 6:
                payTicket(); // thanh toan ve
                break;
            case 7:
            	lockorCancelTicket();
				break; 
            case 9:
                printf("Thoat chuong trinh...!\n"); // ket thuc
                return 0;
            default:
                printf("Loi! Moi ban nhap lai\n"); // lua chon khac
        }
    }
    return 0;
}

void inputString(char *prompt, char *str, int size) { // ham nhap xau
    while (1) {
        printf("%s", prompt);
        if (fgets(str, size, stdin) == NULL) {
            str[0] = '\0';
        } else {
            str[strcspn(str, "\n")] = '\0';
        }
        if (strlen(str) == 0) {
            printf("Khong duoc bo trong.\n");
            continue;
        }
        break;
    }
}

int isTripIdUnique(const char *tripId) { // kiem tra ma chuyen xe co duy nhat khong
    for (int i = 0; i < tripCount; i++) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            return 0;
        }
    }
    return 1;
}

int isEmptyString(const char *str) { // kiem tra xau rong
    return (str == NULL) || (strlen(str) == 0);
}

void showMenu() { // ham hien thi menu
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

int isValidDate(const char* dateStr) {
    int year, month, day, hour, min, sec = 0;

    // C? g?ng d?c d? 6 thành ph?n (bao g?m c? giây, n?u có)
    int count = sscanf(dateStr, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &min, &sec);
    
    // N?u không d?c du?c ít nh?t 5 thành ph?n (YYYY-MM-DD HH:MM), coi là sai d?nh d?ng
    if (count < 5) return 0; 
    
    // N?u d?c du?c 6 thành ph?n, ta ph?i ki?m tra giây
    if (count == 6 && (sec < 0 || sec > 59)) return 0; 

    // --- KI?M TRA PH?M VI NGÀY/THÁNG ---
    
    // 1. Ki?m tra Tháng
    if (month < 1 || month > 12) return 0;

    int maxDay = 31;
    // X? lý tháng 2 và nam nhu?n
    if (month == 2) {
        if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
            maxDay = 29;
        else
            maxDay = 28;
    } 
    // X? lý các tháng 4, 6, 9, 11
    else if (month == 4 || month == 6 || month == 9 || month == 11) {
        maxDay = 30;
    }

    // 2. Ki?m tra Ngày
    if (day < 1 || day > maxDay) return 0;

    // 3. Ki?m tra Gi? và Phút
    if (hour < 0 || hour > 23) return 0;
    if (min < 0 || min > 59) return 0;

    // N?u t?i dây thì h?p l?
    return 1;
}

void addTrip() {
    if (tripCount >= MAX_TRIPS) {
        printf("Them chuyen xe that bai: Danh sach chuyen xe da day.\n");
        return;
    }

    struct Trip newTrip;
    char temp[100];

    // Nhap mã chuyen xe
    while (1) {
        inputString("Nhap ma chuyen xe (duy nhat): ", temp, sizeof(temp));
        if (isEmptyString(temp)) {
            printf("Ma chuyen xe khong duoc de trong. Vui long nhap lai.\n");
            continue;
        }
        if (!isTripIdUnique(temp)) {
            printf("Ma chuyen xe da ton tai. Vui long nhap lai.\n");
            continue;
        }
        strncpy(newTrip.tripId, temp, sizeof(newTrip.tripId)-1);
        newTrip.tripId[sizeof(newTrip.tripId)-1] = '\0';
        break;
    }

    // Nhap tên diem di
    while (1) {
        inputString("Nhap ten diem khoi hanh: ", newTrip.departure.name, sizeof(newTrip.departure.name));
        if (isEmptyString(newTrip.departure.name)) {
            printf("Ten diem khoi hanh khong duoc de trong.\n");
        } else break;
    }
    inputString("Nhap dia chi diem khoi hanh: ", newTrip.departure.address, sizeof(newTrip.departure.address));

    // Nh?p tên di?m d?n
    while (1) {
        inputString("Nhap ten diem den: ", newTrip.destination.name, sizeof(newTrip.destination.name));
        if (isEmptyString(newTrip.destination.name)) {
            printf("Ten diem den khong duoc de trong.\n");
        } else break;
    }
    inputString("Nhap dia chi diem den: ", newTrip.destination.address, sizeof(newTrip.destination.address));

    // Nh?p ngày gi? chuy?n xe có validate tháng
    while (1) {
        inputString("Nhap ngay gio chuyen xe (YYYY-MM-DD HH:MM): ", newTrip.date, sizeof(newTrip.date));
        if (isEmptyString(newTrip.date)) {
            printf("Ngay gio chuyen xe khong duoc de trong.\n");
            continue;
        }
        if (!isValidDate(newTrip.date)) {
            printf("Ngay gio khong hop le hoac thang khong co 31 ngay! Vui long nhap lai.\n");
            continue;
        }
        break;
    }

    // Nh?p t?ng s? gh?
    while (1) {
        inputString("Nhap tong so ghe: ", temp, sizeof(temp));
        if (isEmptyString(temp)) {
            printf("Tong so ghe khong duoc de trong.\n");
            continue;
        }
        newTrip.totalSeats = atoi(temp);
        if (newTrip.totalSeats <= 0) {
            printf("Tong so ghe phai lon hon 0.\n");
            continue;
        }
        break;
    }

    newTrip.bookedSeats = 0;
    trips[tripCount++] = newTrip;
    printf("Them chuyen xe thanh cong!\n");
}

void updateTripInfo() { // ham cap nhat thong tin chuyen xe
    char tripId[20]; // bien ma chuyen xe
    inputString("Nhap ma chuyen xe can cap nhat: ", tripId, sizeof(tripId));

    if (isEmptyString(tripId)) {
        printf("Ma chuyen xe khong duoc de trong.\n");
        return;
    }

    for (int i = 0; i < tripCount; ++i) {
        if (strcmp(trips[i].tripId, tripId) == 0) {
            char newDepartureName[50]; // khoi hanh moi
            char newDepartureAddr[100]; // dia chi khoi hanh moi
            char newDestinationName[50]; // diem den moi
            char newDestinationAddr[100]; // dia chi diem den moi
            char newDate[32]; // ngay gio moi
            char temp[32]; // bien tam
            int newTotalSeats; // tong so ghe moi

            inputString("Nhap ten diem khoi hanh moi: ", newDepartureName, sizeof(newDepartureName));
            if (isEmptyString(newDepartureName)) {
                printf("Ten diem khoi hanh khong duoc de trong.\n");
                return;
            }
            inputString("Nhap dia chi diem khoi hanh moi: ", newDepartureAddr, sizeof(newDepartureAddr));

            inputString("Nhap ten diem den moi: ", newDestinationName, sizeof(newDestinationName));
            if (isEmptyString(newDestinationName)) {
                printf("Ten diem den khong duoc de trong.\n");
                return;
            }
            inputString("Nhap dia chi diem den moi: ", newDestinationAddr, sizeof(newDestinationAddr));

            inputString("Nhap ngay gio moi cho chuyen xe: ", newDate, sizeof(newDate));
            if (isEmptyString(newDate)) {
                printf("Ngay gio chuyen xe khong duoc de trong.\n");
                return;
            }

            while (1) {
                inputString("Nhap tong so ghe moi: ", temp, sizeof(temp));
                if (isEmptyString(temp)) {
                    printf("Tong so ghe khong duoc de trong.\n");
                    continue;
                }
                newTotalSeats = atoi(temp); // chuyen chuoi sang so
                if (newTotalSeats < trips[i].bookedSeats) {
                    printf("Tong so ghe khong duoc nho hon so ghe da dat (%d).\n", trips[i].bookedSeats);
                } else if (newTotalSeats <= 0) {
                    printf("Tong so ghe phai lon hon 0.\n");
                } else {
                    break;
                }
            }

            strncpy(trips[i].departure.name, newDepartureName, sizeof(trips[i].departure.name)-1);
            trips[i].departure.name[sizeof(trips[i].departure.name)-1] = '\0';
            strncpy(trips[i].departure.address, newDepartureAddr, sizeof(trips[i].departure.address)-1);
            trips[i].departure.address[sizeof(trips[i].departure.address)-1] = '\0';
            strncpy(trips[i].destination.name, newDestinationName, sizeof(trips[i].destination.name)-1);
            trips[i].destination.name[sizeof(trips[i].destination.name)-1] = '\0';
            strncpy(trips[i].destination.address, newDestinationAddr, sizeof(trips[i].destination.address)-1);
            trips[i].destination.address[sizeof(trips[i].destination.address)-1] = '\0';
            strncpy(trips[i].date, newDate, sizeof(trips[i].date)-1);
            trips[i].date[sizeof(trips[i].date)-1] = '\0';
            trips[i].totalSeats = newTotalSeats; // cap nhat tong so ghe

            printf("Cap nhat chuyen xe thanh cong\n");
            return;
        }
    }
    printf("Khong tim thay chuyen xe\n");
}

int isValidPhone(const char* phone) { // kiem tra so dien thoai hop le
    int len = strlen(phone);
    if (len < 8 || len > 15)
        return 0;
    for (int i = 0; i < len; ++i)
        if (!isdigit((unsigned char)phone[i]))
            return 0;
    return 1;
}

int findTripIndexById(const char* tripId) { // tim vi tri chuyen xe theo id
    for (int i = 0; i < tripCount; ++i)
        if (strcmp(trips[i].tripId, tripId) == 0)
            return i;
    return -1;
}

int isSeatBooked(const char* tripId, int seatNumber, struct Ticket* tickets, int ticketCount) { // kiem tra so ghe da dat ve chua
    for (int i = 0; i < ticketCount; ++i)
        if (strcmp(tickets[i].tripId, tripId) == 0 && tickets[i].seatNumber == seatNumber)
            return 1;
    return 0;
}

void bookTicket() { // Chuc nang dat ve khong dau
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

// ----- KI?M TRA TÌNH TR?NG VÉ -----
void checkTicketStatus(struct Ticket tickets[], int ticketCount,
                       struct Trip trips[], int tripCount) 
{
    char inputId[32];
    inputString("Nhap ma ve can kiem tra: ", inputId, sizeof(inputId));

    // Lo?i b? kho?ng tr?ng d?u/cu?i
    char *p = inputId;
    while (*p && isspace((unsigned char)*p)) p++; // b? kho?ng tr?ng d?u
    memmove(inputId, p, strlen(p)+1);             // copy ph?n còn l?i
    // b? kho?ng tr?ng cu?i
    int len = strlen(inputId);
    while (len > 0 && isspace((unsigned char)inputId[len-1])) {
        inputId[len-1] = '\0';
        len--;
    }

    if (isEmptyString(inputId)) {
        printf("Ma ve khong duoc de trong!\n");
        return;
    }

    int found = 0;

    for (int i = 0; i < ticketCount; i++) {
        if (strcmp(tickets[i].ticketId, inputId) == 0) {
            found = 1;

            printf("\n=== THONG TIN VE ===\n");
            printf("Ma ve: %s\n", tickets[i].ticketId);
            printf("Ma chuyen xe: %s\n", tickets[i].tripId);
            printf("Hanh khach: %s\n", tickets[i].passenger.name);
            printf("So dien thoai: %s\n", tickets[i].passenger.phone);
            printf("So ghe: %d\n", tickets[i].seatNumber);
            printf("Gia ve: %.2f\n", tickets[i].price);
            printf("Ngay dat: %s\n", tickets[i].date);

            printf("Trang thai thanh toan: ");
            if (tickets[i].paymentStatus == 1)
                printf("Da thanh toan\n");
            else if (tickets[i].paymentStatus == 0)
                printf("Chua thanh toan\n");
            else
                printf("Khong ro\n");

            int tripIndex = findTripIndexById(tickets[i].tripId);
            if (tripIndex != -1) {
                printf("\n=== THONG TIN CHUYEN XE ===\n");
                printf("Diem khoi hanh: %s - %s\n",
                       trips[tripIndex].departure.name,
                       trips[tripIndex].departure.address);
                printf("Diem den: %s - %s\n",
                       trips[tripIndex].destination.name,
                       trips[tripIndex].destination.address);
                printf("Ngay gio chuyen xe: %s\n", trips[tripIndex].date);
                printf("Tong so ghe: %d | Da dat: %d | Con trong: %d\n",
                       trips[tripIndex].totalSeats,
                       trips[tripIndex].bookedSeats,
                       trips[tripIndex].totalSeats - trips[tripIndex].bookedSeats);
            }

            printf("====================================\n\n");
            return;
        }
    }

    if (!found) {
        printf("Khong tim thay ve voi ma: %s\n", inputId);
    }
}
void listTrips() {
    int pageNumber = 1, pageSize = 10;
    char input[32];

    if (tripCount <= 0) {
        printf("Khong co chuyen xe de hien thi\n");
        return;
    }

    // Nhap so dong moi trang (pageSize)
    printf("Nhap so dong moi trang (mac dinh 10, nhan Enter de bo qua): ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &pageSize) != 1 || pageSize <= 0) pageSize = 10;

    // Nhap so trang muon xem (pageNumber)
    printf("Nhap so trang muon xem (mac dinh 1, nhan Enter de bo qua): ");
    fgets(input, sizeof(input), stdin);
    if (sscanf(input, "%d", &pageNumber) != 1 || pageNumber <= 0) pageNumber = 1;

    int totalPages = (tripCount + pageSize - 1) / pageSize;
    if (totalPages <= 0) totalPages = 1; // Phong khi du lieu loi

    while (1) {
        // Kiem tra tinh hop le cua so trang hien tai
        if (pageNumber <= 0 || pageNumber > totalPages) {
            printf("So trang khong hop le. Co tong cong %d trang.\n", totalPages);
            pageNumber = 1;
            continue;
        }

        int startIdx = (pageNumber - 1) * pageSize;
        int endIdx = startIdx + pageSize - 1;
        if (endIdx >= tripCount) endIdx = tripCount - 1;

        // Hien thi tieu de bang
        printf("\n%-10s | %-20s | %-20s | %-19s | %-13s\n", "tripId", "Noi di", "Noi den", "Ngay gio", "Da dat/Tong");
        printf("------------------------------------------------------------------------------------\n");
        // Hien thi du lieu cua trang hien tai
        for (int i = startIdx; i <= endIdx; ++i) {
            printf("%-10s | %-20s | %-20s | %-19s | %3d/%-8d\n",
                   trips[i].tripId,
                   trips[i].departure.name,
                   trips[i].destination.name,
                   trips[i].date,
                   trips[i].bookedSeats,
                   trips[i].totalSeats);
        }
        printf("Trang %d / %d\n", pageNumber, totalPages);

        // Yeu cau chuyen trang
        printf("\nNhap 'n' (Next), 'p' (Previous), 'q' (Quit), hoac so trang de chuyen: ");
        fgets(input, sizeof(input), stdin);
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "q") == 0 || strcmp(input, "Q") == 0) {
            break;
        } else if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0) {
            if (pageNumber < totalPages) pageNumber++;
            else printf("Da o trang cuoi cung!\n");
        } else if (strcmp(input, "p") == 0 || strcmp(input, "P") == 0) {
            if (pageNumber > 1) pageNumber--;
            else printf("Da o trang dau tien!\n");
        } else {
            int newPage = 0;
            if (sscanf(input, "%d", &newPage) == 1) {
                if (newPage > 0 && newPage <= totalPages) pageNumber = newPage;
                else printf("So trang khong hop le.\n");
            } else {
                printf("Lua chon khong hop le.\n");
            }
        }
    }
}

// Ham tim kiem ve theo ticketId (khong dau)
int findTicketIndexById(const char* ticketId) {
    for (int i = 0; i < ticketCount; ++i) {
        if (strcmp(tickets[i].ticketId, ticketId) == 0) {
            return i;
        }
    }
    return -1;
}

// Chuc nang thanh toan 
void payTicket() {
    char ticketId[25];
    printf("Nhap ma ve can thanh toan: ");
    fgets(ticketId, sizeof(ticketId), stdin);
    ticketId[strcspn(ticketId, "\n")] = 0; // Xoa ky tu newline

    // Validation: ticketId khong duoc de trong
    if (strlen(ticketId) == 0) {
        printf("Ma ve khong duoc de trong\n");
        return;
    }

    // Tim vi tri ve theo ticketId
    int idx = findTicketIndexById(ticketId);

    // Validation: ticketId phai ton tai trong danh sach ve
    if (idx == -1) {
        printf("Khong tim thay ve\n");
        return;
    }

    // Validation: Ve chua bi khoa/huy moi duoc thanh toan (status = 0: hop le, 1: da huy, 2: da khoa)
    if (tickets[idx].status == 1 || tickets[idx].status == 2) {
        printf("Ve khong the thanh toan vi da bi vo hieu hoa\n");
        return;
    }

    // Validation: paymentStatus phai = 0 (chua thanh toan)
    if (tickets[idx].paymentStatus == 1) {
        printf("Ve da thanh toan truoc do\n");
        return;
    }

    // Logic xu ly thanh toan: hop le -> cap nhat paymentStatus = 1
    tickets[idx].paymentStatus = 1;
    printf("Thanh toan thanh cong\n");
}
// Prototype (d?t ? trên, tru?c main)
void lockorCancelTicket(void) {
    char inputId[32];
    char actionStr[10];
    int action;

    inputString("Nhap ticketId: ", inputId, sizeof(inputId));
    if (isEmptyString(inputId)) {
        printf("ticketId khong duoc de trong!\n");
        return;
    }

    printf("Chon hanh dong (1 = Khoa ve, 2 = Huy ve): ");
    if (fgets(actionStr, sizeof(actionStr), stdin) == NULL) {
        printf("Nhap khong hop le!\n");
        return;
    }
    action = atoi(actionStr);

    if (action != 1 && action != 2) {
        printf("Hanh dong khong hop le!\n");
        return;
    }

    int idx = findTicketIndexById(inputId);
    if (idx == -1) {
        printf("Khong tim thay ve!\n");
        return;
    }

    struct Ticket *tk = &tickets[idx];

    // Da bi canceled hoac locked
    if (tk->status == 1) { // canceled
        printf("Ve da bi huy!\n");
        return;
    }
    if (tk->status == 2) { // locked
        printf("Ve da bi khoa!\n");
        return;
    }

    // KHÓA VÉ
    if (action == 1) {
        tk->status = 2; // locked
        printf("Khoa ve thanh cong!\n");
        return;
    }

    // H?Y VÉ
    if (tk->paymentStatus == 1) {
        printf("Khong the huy vi ve da thanh toan!\n");
        return;
    }

    int tripIndex = findTripIndexById(tk->tripId);
    if (tripIndex != -1 && trips[tripIndex].bookedSeats > 0) {
        trips[tripIndex].bookedSeats--;
    }

    tk->status = 1; // canceled
    printf("Huy ve thanh cong!\n");
}
